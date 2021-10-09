#include "gamemainloop.h"

//includes copied from main.cpp
#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <unordered_map>
#include <stdexcept>

//GPU rendering:
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "shader.h"
#include "renderer.h"
#include <glm.hpp> //GLM is overkill but that's okay
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>

//important stuff:
#include "rng.h"
#include "colorvalueholder.h"
#include "backgroundrect.h"
#include "tank.h"
#include "wall.h"
#include "bullet.h"
#include "powersquare.h"
#include "level.h"
#include "circlehazard.h"
#include "recthazard.h"

//managers:
#include "developermanager.h"
#include "gamemanager.h"
#include "keypressmanager.h"
#include "tankmanager.h"
#include "bulletmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "levelmanager.h"
#include "hazardmanager.h"

//classes with important handling functions:
#include "collisionhandler.h"
#include "resetthings.h"
#include "priorityhandler.h"
#include "colormixer.h"
#include "powerfunctionhelper.h"
#include "endgamehandler.h"

#include "diagnostics.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

bool GameMainLoop::currentlyDrawing = false; //look into std::mutex
long GameMainLoop::frameCount = 0; //doesn't need a long for how it's interpreted...
long GameMainLoop::ticksUntilFrame = 1; //whatever again
int GameMainLoop::physicsRate = 100; //(in Hz)

void doThing() {
	return;
}

void GameMainLoop::Tick(int physicsUPS) {
	//while (currentlyDrawing) {}

	auto start = Diagnostics::getTime();
	doThing();

	//move tanks:
	moveTanks();

	//collide tanks with powerups:
	Diagnostics::startTiming("tank to powerups");
	tankToPowerup();
	Diagnostics::endTiming();
	
	//tick hazards:
	tickHazards();
	
	//move bullets:
	moveBullets();

	//powerCalculate on tanks and bullets, then tank shoot:
	Diagnostics::startTiming("powerCalculate and tank shoot");
	tankPowerCalculate();
	bulletPowerCalculate();
	tankShoot();
	//cout << BulletManager::getNumBullets() << endl;
	Diagnostics::endTiming();

	//collide tanks with walls:
	//this comes before powerCalculate stuff in JS Tanks; TODO: should this be changed?
	Diagnostics::startTiming("tank to walls");
	tankToWall();
	Diagnostics::endTiming();

	//collide tanks with hazards:
	Diagnostics::startTiming("tank-hazards");
	tankToHazard();
	Diagnostics::endTiming();

	//collide tanks with tanks:
	Diagnostics::startTiming("tank-tank");
	tankToTank();
	Diagnostics::endTiming();

	//collide tanks against edges:
	tankToEdge();
	
	//collide bullets against edges:
	Diagnostics::startTiming("bullet-edge");
	bulletToEdge();
	Diagnostics::endTiming();

	//collide bullets with walls:
	Diagnostics::startTiming("bullet-wall");
	bulletToWall();
	Diagnostics::endTiming();
	//bullet to wall is a big timesink, but it can only really be sped up by multithreading

	//collide bullets with hazards:
	Diagnostics::startTiming("bullet-hazards");
	bulletToHazard();
	Diagnostics::endTiming();
	//probably another big timesink, but there aren't many hazards

	//collide bullets with bullets:
	Diagnostics::startTiming("bullet-bullet");
	bulletToBullet();
	Diagnostics::endTiming();
	//add another shader: main uses proj, modify doesn't
	//bullet to bullet collision is the biggest timesink (obviously)
	//unfortunately it can only be O(n^2), and multithreading doesn't seem like it would work

	//collide bullets with tanks:
	Diagnostics::startTiming("bullet-tank");
	bulletToTank();
	Diagnostics::endTiming();

	//edge constrain tanks again in case bullet decided to move tank
	//don't edge constrain if said tank is dead //TODO: implement
	/*
	tankToEdge();
	*/

	//finish up by incrementing the tick count
	GameManager::Tick();

	auto end = Diagnostics::getTime();

	//Diagnostics::printPreciseTimings();
	Diagnostics::clearTimes();

	//std::cout << "tick: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << endl << endl;

	if (!EndGameHandler::shouldGameEnd()) {
		glutTimerFunc(1000/physicsUPS, GameMainLoop::Tick, physicsUPS);
		if (frameCount == 0) {
			glutPostRedisplay();
		}
		++frameCount %= ticksUntilFrame;
	} else {
		glutPostRedisplay();
		glutTimerFunc(1000, ResetThings::reset, 0);
		glutTimerFunc(1000 + 1000/physicsUPS, GameMainLoop::Tick, physicsUPS);
	}
}

void GameMainLoop::moveTanks() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->move();
	}
}

void GameMainLoop::tankToPowerup() {
	for (int i = PowerupManager::getNumPowerups() - 1; i >= 0; i--) {
		PowerSquare* p = PowerupManager::getPowerup(i);

		for (int j = 0; j < TankManager::getNumTanks(); j++) {
			Tank* t = TankManager::getTank(j);
			if (CollisionHandler::partiallyCollided(p, t)) {
				p->givePower(t);
				PowerupManager::deletePowerup(i);
				break;
			}
		}
	}
}

void GameMainLoop::tickHazards() {
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		HazardManager::getCircleHazard(i)->tick();
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		HazardManager::getRectHazard(i)->tick();
	}
}

void GameMainLoop::moveBullets() {
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b = BulletManager::getBullet(i);
		bool shouldBeKilled = false;
		bool modifiedMovement = false;
		bool overridedMovement = false;
		bool noMoreMovementSpecials = false;

		for (int k = 0; k < b->bulletPowers.size(); k++) {
			if (b->bulletPowers[k]->modifiesMovement) {
				if (b->bulletPowers[k]->modifiedMovementCanOnlyWorkIndividually && modifiedMovement) {
					continue;
				}
				if (noMoreMovementSpecials) {
					continue;
				}

				modifiedMovement = true;
				if (b->bulletPowers[k]->overridesMovement) {
					overridedMovement = true;
				}
				if (!b->bulletPowers[k]->modifiedMovementCanWorkWithOthers) {
					noMoreMovementSpecials = true;
				}

				InteractionBoolHolder check_temp = b->bulletPowers[k]->modifiedMovement(b);
				if (check_temp.shouldDie) {
					shouldBeKilled = true;
					overridedMovement = true;
					break;
				}
			}
		}

		if (!overridedMovement) {
			b->move();
		}

		if (shouldBeKilled) {
			BulletManager::deleteBullet(i);
			continue;
		}
	}
	BulletManager::forceLimitBullets();
}

void GameMainLoop::tankPowerCalculate() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->powerCalculate();
	}
}

void GameMainLoop::bulletPowerCalculate() {
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b = BulletManager::getBullet(i);
		b->powerCalculate();
		if (b->isDead()) {
			BulletManager::deleteBullet(i);
			continue;
		}
	}
}

void GameMainLoop::tankShoot() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->shoot();
	}
}

void GameMainLoop::tankToWall() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		Tank* t = TankManager::getTank(i);
		bool shouldBeKilled = false; //maybe the walls are poison with a certain powerup? I dunno, but gotta have it as an option

		for (int j = WallManager::getNumWalls() - 1; j >= 0; j--) {
			Wall* w = WallManager::getWall(j);
			bool killWall = false;
			bool modifiedWallCollision = false;
			bool overridedWallCollision = false;
			bool noMoreWallCollisionSpecials = false;

			if (CollisionHandler::partiallyCollided(t, w)) {
				for (int k = 0; k < t->tankPowers.size(); k++) {
					if (t->tankPowers[k]->modifiesCollisionWithWall) {
						if (t->tankPowers[k]->modifiedCollisionWithWallCanOnlyWorkIndividually && modifiedWallCollision) {
							continue;
						}
						if (noMoreWallCollisionSpecials) {
							continue;
						}

						modifiedWallCollision = true;
						if (t->tankPowers[k]->overridesCollisionWithWall) {
							overridedWallCollision = true;
						}
						if (!t->tankPowers[k]->modifiedEdgeCollisionCanWorkWithOthers) {
							noMoreWallCollisionSpecials = true;
						}

						InteractionBoolHolder check_temp = t->tankPowers[k]->modifiedCollisionWithWall(t, w);
						if (check_temp.shouldDie) {
							shouldBeKilled = true;
						}
						if (check_temp.otherShouldDie) {
							killWall = true;
						}
					}
				}

				if (!overridedWallCollision) {
					CollisionHandler::pushMovableAwayFromImmovable(t, w);
				}
			}

			if (killWall) {
				WallManager::deleteWall(j);
			}
		}

		if (shouldBeKilled) {
			//TODO: proper implementation?
		}

	}
}

void GameMainLoop::tankToHazard() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		Tank* t = TankManager::getTank(i);
		bool shouldBeKilled = false;

		//circles:
		for (int j = HazardManager::getNumCircleHazards() - 1; j >= 0; j--) {
			CircleHazard* ch = HazardManager::getCircleHazard(j);
			bool killCircleHazard = false;
			bool modifiedCircleHazardCollision = false;
			bool overridedCircleHazardCollision = false;
			bool noMoreCircleHazardCollisionSpecials = false;

			if (CollisionHandler::partiallyCollided(t, ch)) {
				//tankpower decides whether to use the partial collision or the true collision
				for (int k = 0; k < t->tankPowers.size(); k++) {
					if (t->tankPowers[k]->modifiesCollisionWithCircleHazard) {
						if (t->tankPowers[k]->modifiedCollisionWithCircleHazardCanOnlyWorkIndividually && modifiedCircleHazardCollision) {
							continue;
						}
						if (noMoreCircleHazardCollisionSpecials) {
							continue;
						}

						modifiedCircleHazardCollision = true;
						if (t->tankPowers[k]->overridesCollisionWithCircleHazard) {
							overridedCircleHazardCollision = true;
						}
						if (!t->tankPowers[k]->modifiedCollisionWithCircleHazardCanWorkWithOthers) {
							noMoreCircleHazardCollisionSpecials = true;
						}

						InteractionBoolHolder check_temp = t->tankPowers[k]->modifiedCollisionWithCircleHazard(t, ch);
						if (check_temp.shouldDie) {
							shouldBeKilled = true;
						}
						if (check_temp.otherShouldDie) {
							killCircleHazard = true;
						}
					}
				}

				if (!overridedCircleHazardCollision) {
					if (CollisionHandler::partiallyCollided(t, ch)) {
						if (ch->actuallyCollided(t)) {
							InteractionBoolHolder result = EndGameHandler::determineWinner(t, ch);
							shouldBeKilled = result.shouldDie;
							killCircleHazard = result.otherShouldDie;
						}
					}
				}
			}

			if (killCircleHazard) {
				HazardManager::deleteCircleHazard(j);
			}
		}

		if (shouldBeKilled) {
			//TODO: proper implementation?
			//continue;
		}
		
		//rectangles:
		for (int j = 0; j < HazardManager::getNumRectHazards(); j++) {
			bool modifiedRectHazardCollision = false;
			bool overridedRectHazardCollision = false;
			bool noMoreRectHazardCollisionSpecials = false;
			bool killRectHazard = false;
			RectHazard* rh = HazardManager::getRectHazard(j);

			if (CollisionHandler::partiallyCollided(t, rh)) {
				for (int k = 0; k < t->tankPowers.size(); k++) {
					if (t->tankPowers[k]->modifiesCollisionWithRectHazard) {
						if (t->tankPowers[k]->modifiedCollisionWithRectHazardCanOnlyWorkIndividually && modifiedRectHazardCollision) {
							continue;
						}
						if (noMoreRectHazardCollisionSpecials) {
							continue;
						}

						modifiedRectHazardCollision = true;
						if (t->tankPowers[k]->overridesCollisionWithRectHazard) {
							overridedRectHazardCollision = true;
						}
						if (!t->tankPowers[k]->modifiedCollisionWithRectHazardCanWorkWithOthers) {
							noMoreRectHazardCollisionSpecials = true;
						}

						InteractionBoolHolder check_temp = t->tankPowers[k]->modifiedCollisionWithRectHazard(t, rh);
						if (check_temp.shouldDie) {
							shouldBeKilled = true;
						}
						if (check_temp.otherShouldDie) {
							killRectHazard = true;
						}
					}
				}

				if (!overridedRectHazardCollision) {
					if (CollisionHandler::partiallyCollided(t, rh)) {
						if (rh->actuallyCollided(t)) {
							InteractionBoolHolder result = EndGameHandler::determineWinner(t, rh);
							shouldBeKilled = result.shouldDie;
							killRectHazard = result.otherShouldDie;
						}
					}
				}
			}

			if (killRectHazard) {
				HazardManager::deleteRectHazard(j);
			}
		}

		if (shouldBeKilled) {
			//TODO: proper implementation?
		}
	}
}

void GameMainLoop::tankToTank() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		Tank* t_outer = TankManager::getTank(i);
		bool t_outerShouldDie = false;

		for (int j = 0; j < TankManager::getNumTanks(); j++) {
			if (i == j) {
				continue;
			}

			Tank* t_inner = TankManager::getTank(j);
			bool t_innerShouldDie = false;
			bool modifiedTankCollision = false;
			bool overridedTankCollision = false;
			bool noMoreTankCollisionSpecials = false;

			if (CollisionHandler::partiallyCollided(t_outer, t_inner)) {
				for (int k = 0; k < t_outer->tankPowers.size(); k++) {
					if (t_outer->tankPowers[k]->modifiesCollisionWithWall) {
						if (t_outer->tankPowers[k]->modifiedCollisionWithTankCanOnlyWorkIndividually && modifiedTankCollision) {
							continue;
						}
						if (noMoreTankCollisionSpecials) {
							continue;
						}

						modifiedTankCollision = true;
						if (t_outer->tankPowers[k]->overridesCollisionWithTank) {
							overridedTankCollision = true;
						}
						if (!t_outer->tankPowers[k]->modifiedCollisionWithTankCanWorkWithOthers) {
							noMoreTankCollisionSpecials = true;
						}

						InteractionBoolHolder check_temp = t_outer->tankPowers[k]->modifiedCollisionWithTank(t_outer, t_inner);
						if (check_temp.shouldDie) {
							t_outerShouldDie = true;
						}
						if (check_temp.otherShouldDie) {
							if (t_outer->getTeamID() != t_inner->getTeamID()) {
								t_innerShouldDie = true;
							}
						}
					}
				}

				if (!overridedTankCollision) {
					InteractionBoolHolder result = EndGameHandler::determineWinner(t_outer, t_inner);
					t_outerShouldDie = result.shouldDie;
					t_innerShouldDie = result.otherShouldDie;
				}
			}

			if (t_innerShouldDie) {
				//TODO: proper implementation?
			}
		}

		if (t_outerShouldDie) {
			//TODO: proper implementation?
		}
	}
}

void GameMainLoop::tankToEdge() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		Tank* t = TankManager::getTank(i);
		bool shouldBeKilled = false;
		bool modifiedEdgeCollision = false;
		bool overridedEdgeCollision = false;
		bool noMoreEdgeCollisionSpecials = false;

		if (CollisionHandler::partiallyOutOfBounds(t)) {
			for (int k = 0; k < t->tankPowers.size(); k++) {
				if (t->tankPowers[k]->modifiesEdgeCollision) {
					if (t->tankPowers[k]->modifiedEdgeCollisionCanOnlyWorkIndividually && modifiedEdgeCollision) {
						continue;
					}
					if (noMoreEdgeCollisionSpecials) {
						continue;
					}

					modifiedEdgeCollision = true;
					if (t->tankPowers[k]->overridesEdgeCollision) {
						overridedEdgeCollision = true;
					}
					if (!t->tankPowers[k]->modifiedEdgeCollisionCanWorkWithOthers) {
						noMoreEdgeCollisionSpecials = true;
					}

					InteractionBoolHolder check_temp = t->tankPowers[k]->modifiedEdgeCollision(t);
					if (check_temp.shouldDie) {
						shouldBeKilled = true;
					}
				}
			}

			if (!overridedEdgeCollision) {
				CollisionHandler::edgeConstrain(t);
			}
		}

		if (shouldBeKilled) {
			//TODO: proper implementation?
		}
	}
}

void GameMainLoop::bulletToEdge() {
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b = BulletManager::getBullet(i);
		bool shouldBeKilled = false;
		bool modifiedEdgeCollision = false;
		bool overridedEdgeCollision = false;
		bool noMoreEdgeCollisionSpecials = false;

		if (CollisionHandler::partiallyOutOfBounds(b)) {
			for (int k = 0; k < b->bulletPowers.size(); k++) {
				if (b->bulletPowers[k]->modifiesEdgeCollision) {
					if (b->bulletPowers[k]->modifiedEdgeCollisionCanOnlyWorkIndividually && modifiedEdgeCollision) {
						continue;
					}
					if (noMoreEdgeCollisionSpecials) {
						continue;
					}

					modifiedEdgeCollision = true;
					if (b->bulletPowers[k]->overridesEdgeCollision) {
						overridedEdgeCollision = true;
					}
					if (!b->bulletPowers[k]->modifiedEdgeCollisionCanWorkWithOthers) {
						noMoreEdgeCollisionSpecials = true;
					}

					InteractionBoolHolder check_temp = b->bulletPowers[k]->modifiedEdgeCollision(b);
					if (check_temp.shouldDie) {
						shouldBeKilled = true;
					}
				}
			}

			if (!overridedEdgeCollision) {
				if (CollisionHandler::fullyOutOfBounds(b)) {
					shouldBeKilled = true;
				}
			}
		}

		if (shouldBeKilled) {
			BulletManager::deleteBullet(i);
			continue;
		}
	}
}

void GameMainLoop::bulletToWall() {
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b = BulletManager::getBullet(i);
		bool shouldBeKilled = false;

		for (int j = WallManager::getNumWalls() - 1; j >= 0; j--) {
			Wall* w = WallManager::getWall(j);
			bool killWall = false;
			bool modifiedWallCollision = false;
			bool overridedWallCollision = false;
			bool noMoreWallCollisionSpecials = false;

			if (CollisionHandler::partiallyCollided(b, w)) {
				for (int k = 0; k < b->bulletPowers.size(); k++) {
					if (b->bulletPowers[k]->modifiesCollisionWithWall) {
						if (b->bulletPowers[k]->modifiedCollisionWithWallCanOnlyWorkIndividually && modifiedWallCollision) {
							continue;
						}
						if (noMoreWallCollisionSpecials) {
							continue;
						}

						modifiedWallCollision = true;
						if (b->bulletPowers[k]->overridesCollisionWithWall) {
							overridedWallCollision = true;
						}
						if (!b->bulletPowers[k]->modifiedCollisionWithWallCanWorkWithOthers) {
							noMoreWallCollisionSpecials = true;
						}

						InteractionBoolHolder check_temp = b->bulletPowers[k]->modifiedCollisionWithWall(b, w);
						if (check_temp.shouldDie) {
							shouldBeKilled = true;
						}
						if (check_temp.otherShouldDie) {
							killWall = true;
						}
					}
				}

				if (!overridedWallCollision) {
					if (CollisionHandler::partiallyCollided(b, w)) {
						shouldBeKilled = true;
					}
				}
			}

			if (killWall) {
				WallManager::deleteWall(j);
			}
		}

		if (shouldBeKilled) {
			//shouldBeKilled = b->kill();
			//if (shouldBeKilled) {
				BulletManager::deleteBullet(i);
			//}
		}
	}
}

void GameMainLoop::bulletToHazard() {
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b = BulletManager::getBullet(i);
		bool shouldBeKilled = false;

		//circles:
		for (int j = HazardManager::getNumCircleHazards() - 1; j >= 0; j--) {
			CircleHazard* ch = HazardManager::getCircleHazard(j);
			bool killCircleHazard = false;
			bool modifiedCircleHazardCollision = false;
			bool overridedCircleHazardCollision = false;
			bool noMoreCircleHazardCollisionSpecials = false;

			if (!b->canCollideWith(ch)) {
				continue;
			}
			if (CollisionHandler::partiallyCollided(b, ch)) {
				for (int k = 0; k < b->bulletPowers.size(); k++) {
					if (b->bulletPowers[k]->modifiesCollisionWithCircleHazard) {
						if (b->bulletPowers[k]->modifiedCollisionWithCircleHazardCanOnlyWorkIndividually && modifiedCircleHazardCollision) {
							continue;
						}
						if (noMoreCircleHazardCollisionSpecials) {
							continue;
						}

						modifiedCircleHazardCollision = true;
						if (b->bulletPowers[k]->overridesCollisionWithCircleHazard) {
							overridedCircleHazardCollision = true;
						}
						if (!b->bulletPowers[k]->modifiedCollisionWithCircleHazardCanWorkWithOthers) {
							noMoreCircleHazardCollisionSpecials = true;
						}

						InteractionBoolHolder check_temp = b->bulletPowers[k]->modifiedCollisionWithCircleHazard(b, ch);
						if (check_temp.shouldDie) {
							shouldBeKilled = true;
						}
						if (check_temp.otherShouldDie) {
							killCircleHazard = true;
						}
					}
				}

				if (!overridedCircleHazardCollision) {
					if (CollisionHandler::partiallyCollided(b, ch)) {
						if (ch->actuallyCollided(b)) {
							InteractionBoolHolder result = EndGameHandler::determineWinner(b, ch);
							shouldBeKilled = result.shouldDie;
							killCircleHazard = result.otherShouldDie;
						}
					}
				}
			}

			if (killCircleHazard) {
				HazardManager::deleteCircleHazard(j);
			}
		}

		if (shouldBeKilled) {
			//shouldBeKilled = b->kill();
			//if (shouldBeKilled) {
				BulletManager::deleteBullet(i);
				continue; //bullet died, can't do any more collision with it
			//}
		}
		
		//rectangles:
		for (int j = 0; j < HazardManager::getNumRectHazards(); j++) {
			RectHazard* rh = HazardManager::getRectHazard(j);
			bool killRectHazard = false;
			bool modifiedRectHazardCollision = false;
			bool overridedRectHazardCollision = false;
			bool noMoreRectHazardCollisionSpecials = false;

			if (!b->canCollideWith(rh)) {
				continue;
			}
			if (CollisionHandler::partiallyCollided(b, rh)) {
				for (int k = 0; k < b->bulletPowers.size(); k++) {
					if (b->bulletPowers[k]->modifiesCollisionWithRectHazard) {
						if (b->bulletPowers[k]->modifiedCollisionWithRectHazardCanOnlyWorkIndividually && modifiedRectHazardCollision) {
							continue;
						}
						if (noMoreRectHazardCollisionSpecials) {
							continue;
						}

						modifiedRectHazardCollision = true;
						if (b->bulletPowers[k]->overridesCollisionWithRectHazard) {
							overridedRectHazardCollision = true;
						}
						if (!b->bulletPowers[k]->modifiedCollisionWithRectHazardCanWorkWithOthers) {
							noMoreRectHazardCollisionSpecials = true;
						}

						InteractionBoolHolder check_temp = b->bulletPowers[k]->modifiedCollisionWithRectHazard(b, rh);
						if (check_temp.shouldDie) {
							shouldBeKilled = true;
						}
						if (check_temp.otherShouldDie) {
							killRectHazard = true;
						}
					}
				}

				if (!overridedRectHazardCollision) {
					if (CollisionHandler::partiallyCollided(b, rh)) {
						if (rh->actuallyCollided(b)) {
							InteractionBoolHolder result = EndGameHandler::determineWinner(b, rh);
							shouldBeKilled = result.shouldDie;
							killRectHazard = result.otherShouldDie;
						}
					}
				}
			}

			if (killRectHazard) {
				HazardManager::deleteRectHazard(j);
			}
		}

		if (shouldBeKilled) {
			//shouldBeKilled = b->kill();
			//if (shouldBeKilled) {
				BulletManager::deleteBullet(i);
			//}
		}
	}
}

void GameMainLoop::bulletToBullet() {
	//TODO: modernize (add default vs custom collision stuff)
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b_outer = BulletManager::getBullet(i);
		bool b_outerShouldDie = false;

		for (int j = BulletManager::getNumBullets() - 1; j >= 0; j--) { //could start at i-1? //TODO: find out
			Bullet* b_inner = BulletManager::getBullet(j);
			bool b_innerShouldDie = false;

			if (b_outer == b_inner) {
				continue;
			}
			if (!b_outer->canCollideWith(b_inner)) {
				continue;
			}
			if (CollisionHandler::partiallyCollided(b_outer, b_inner)) {
				InteractionBoolHolder result = EndGameHandler::determineWinner(b_outer, b_inner);
				//if (!b_outerShouldDie) {
					b_outerShouldDie = result.shouldDie; //TODO: does this need to go in the conditional?
				//}
				b_innerShouldDie = result.otherShouldDie;

				if (b_innerShouldDie) {
					b_innerShouldDie = b_inner->kill();
					if (b_innerShouldDie) {
						BulletManager::deleteBullet(j);
						if (i > j) {
							i--;
						}
					}
				}
			}
		}

		if (b_outerShouldDie) {
			b_outerShouldDie = b_outer->kill();
			if (b_outerShouldDie) {
				BulletManager::deleteBullet(i);
			}
		}
	}
}

void GameMainLoop::bulletToTank() {
	//TODO: modernize (add default vs custom collision stuff)
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		Tank* t = TankManager::getTank(i);
		bool shouldBeKilled = false;

		for (int j = BulletManager::getNumBullets() - 1; j >= 0; j--) {
			Bullet* b = BulletManager::getBullet(j);
			bool killBullet = false;

			if (!b->canCollideWith(t)) {
				continue;
			}
			if (CollisionHandler::partiallyCollided(t, b)) {
				InteractionBoolHolder result = EndGameHandler::determineWinner(t, b);
				shouldBeKilled = result.shouldDie;
				killBullet = result.otherShouldDie;

				//if (killBullet) {
					//killBullet = b->kill();
					//if (killBullet) {
						BulletManager::deleteBullet(j);
					//}
					//the bullet needs to be killed
				//}
			}
		}

		if (shouldBeKilled) {
			//TODO: proper implementation?
		}
	}
}

void GameMainLoop::drawEverything() {
	GameMainLoop::currentlyDrawing = true;
	
	auto start = Diagnostics::getTime();

	Diagnostics::startTiming("clear");
	Renderer::BeginningStuff();
	Renderer::Clear();
	Diagnostics::endTiming();
	
	Diagnostics::startTiming("background rect");
	BackgroundRect::draw();
	Renderer::UnbindAll(); //I honestly don't know if this is needed anymore but it doesn't hurt performance too much so it can stay
	Diagnostics::endTiming();
	

	//is this needed? //ehh it can stay, may be needed for emergency CPU drawings
	// Set up the transformations stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	Diagnostics::startTiming("powerups");
	for (int i = 0; i < PowerupManager::getNumPowerups(); i++) {
		PowerupManager::getPowerup(i)->draw();
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("hazards");
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		HazardManager::getCircleHazard(i)->draw();
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		HazardManager::getRectHazard(i)->draw();
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("walls");
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		WallManager::getWall(i)->draw();
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();
	
	Diagnostics::startTiming("bullets");
	for (int i = 0; i < BulletManager::getNumBullets(); i++) {
		BulletManager::getBullet(i)->draw();
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	//drawing text on the GPU will need a library, so names don't get drawn anymore
	/*
	for (int i = 0; i < tanks.size(); i++) {
		tanks[i]->drawName();
	}
	*/

	Diagnostics::startTiming("tanks");
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->draw();
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("flush");

	Renderer::Cleanup(); //possibly put glFlush/glutSwapBuffers in this

	//for single framebuffer, use glFlush; for double framebuffer, swap the buffers
	//swapping buffers is limited to monitor refresh rate, so I use glFlush
	glFlush();
	//glutSwapBuffers();

	Diagnostics::endTiming();

	auto end = Diagnostics::getTime();

	//Diagnostics::printPreciseTimings();
	Diagnostics::clearTimes();

	//std::cout << "entire: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << endl << endl;

	currentlyDrawing = false;
}
