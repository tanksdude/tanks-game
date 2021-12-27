#include "game-main-loop.h"

//includes copied from main.cpp
#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <unordered_map>
#include <stdexcept>

//GPU rendering:
#include "vertex-array.h"
#include "vertex-buffer.h"
#include "index-buffer.h"
#include "shader.h"
#include "renderer.h"
#include <glm.hpp> //GLM is overkill but that's okay
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>

//important stuff:
#include "rng.h"
#include "color-value-holder.h"
#include "background-rect.h"
#include "tank.h"
#include "wall.h"
#include "bullet.h"
#include "power-square.h"
#include "level.h"
#include "circle-hazard.h"
#include "rect-hazard.h"

//managers:
#include "developer-manager.h"
#include "game-manager.h"
#include "keypress-manager.h"
#include "tank-manager.h"
#include "bullet-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "level-manager.h"
#include "hazard-manager.h"
#include "diagnostics.h"

//classes with important handling functions:
#include "collision-handler.h"
#include "reset-things.h"
#include "priority-handler.h"
#include "color-mixer.h"
#include "power-function-helper.h"
#include "end-game-handler.h"
#include "physics-handler.h"

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

	//level stuff:
	levelTick();

	//move tanks:
	moveTanks();

	//collide tanks with powerups:
	Diagnostics::startTiming("tank-powerups");
	tankToPowerup();
	Diagnostics::endTiming();

	//tick hazards:
	tickHazards();

	//move bullets:
	moveBullets();

	//powerCalculate on tanks and bullets, then tank shoot:
	Diagnostics::startTiming("power calculate and tank shoot");
	tankPowerCalculate();
	bulletPowerCalculate();
	tankShoot();
	//cout << BulletManager::getNumBullets() << endl;
	Diagnostics::endTiming();

	//collide tanks with walls:
	//this comes before powerCalculate stuff in JS Tanks; TODO: should this be changed?
	Diagnostics::startTiming("tank-walls");
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

	//collide bullets with hazards:
	Diagnostics::startTiming("bullet-hazards");
	bulletToHazard();
	Diagnostics::endTiming();

	//collide bullets with bullets:
	Diagnostics::startTiming("bullet-bullet");
	bulletToBullet();
	Diagnostics::endTiming();
	//add another shader: main uses proj, modify doesn't

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
	Diagnostics::pushGraphTime("tick", Diagnostics::getDiff(start, end));

	//Diagnostics::printPreciseTimings();
	Diagnostics::clearTimes();

	//std::cout << BulletManager::getNumBullets() << std::endl;
	//std::cout << "tick: " << (long long)Diagnostics::getDiff(start, end) << "ms" << std::endl << std::endl;

	//largest timesinks:
	//1. bullet-bullet
	//2. bullet-hazard
	//2. bullet-wall (really depends on how many hazards/walls)
	//3. power calculate and tank shoot

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

void GameMainLoop::levelTick() {
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->tickLevelEffects();
	}
	//for (int i = 0; i < LevelManager::getNumLevels(); i++) {
	//	LevelManager::getLevel(i)->doLevelEffects();
	//}
	//TODO: ^^^ handled by level?
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->tick();
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
					if (t->tankPowers[k]->getModifiesCollisionWithCircleHazard(ch)) {
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
					if (t->tankPowers[k]->getModifiesCollisionWithRectHazard(rh)) {
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
	//broad phase
	std::vector<std::pair<int, int>> collisionList = PhysicsHandler::sweepAndPrune<Circle*, Rect*>(BulletManager::getBulletCollisionList(), WallManager::getWallCollisionList());

	//narrow phase and resolve collision
	std::vector<Game_ID> bulletDeletionList;
	std::vector<Game_ID> wallDeletionList;
	for (int i = 0; i < collisionList.size(); i++) {
		std::pair<int, int> collisionPair = collisionList[i];
		Bullet* b = BulletManager::getBullet(collisionPair.first);
		bool shouldBeKilled = false;

		Wall* w = WallManager::getWall(collisionPair.second);
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
			//wallDeletionList.push_back(collisionPair.second);
			wallDeletionList.push_back(w->getGameID());
		}

		if (shouldBeKilled) {
			//shouldBeKilled = b->kill();
			//if (shouldBeKilled) {
				//bulletDeletionList.push_back(collisionPair.first);
				bulletDeletionList.push_back(b->getGameID());
			//}
		}
	}

	//clear deaths
	//std::sort(bulletDeletionList.begin(), bulletDeletionList.end());
	for (int i = bulletDeletionList.size() - 1; i >= 0; i--) {
		//BulletManager::deleteBullet(bulletDeletionList[i]);
		BulletManager::deleteBulletByID(bulletDeletionList[i]);
	}
	//std::sort(wallDeletionList.begin(), wallDeletionList.end());
	for (int i = wallDeletionList.size() - 1; i >= 0; i--) {
		WallManager::deleteWallByID(wallDeletionList[i]);
	}
}

void GameMainLoop::bulletToHazard() {
	//broad phase
	std::vector<std::pair<int, int>> collisionList = PhysicsHandler::sweepAndPrune<Circle*, Circle*>(BulletManager::getBulletCollisionList(), HazardManager::getCircleHazardCollisionList());

	//narrow phase and resolve collision
	std::vector<Game_ID> bulletDeletionList;
	std::vector<Game_ID> circleHazardDeletionList;
	for (int i = 0; i < collisionList.size(); i++) {
		std::pair<int, int> collisionPair = collisionList[i];
		Bullet* b = BulletManager::getBullet(collisionPair.first);
		bool shouldBeKilled = false;

		//circles:
		CircleHazard* ch = HazardManager::getCircleHazard(collisionPair.second);
		bool killCircleHazard = false;
		bool modifiedCircleHazardCollision = false;
		bool overridedCircleHazardCollision = false;
		bool noMoreCircleHazardCollisionSpecials = false;

		if (!b->canCollideWith(ch)) {
			continue;
		}
		if (CollisionHandler::partiallyCollided(b, ch)) {
			for (int k = 0; k < b->bulletPowers.size(); k++) {
				if (b->bulletPowers[k]->getModifiesCollisionWithCircleHazard(ch)) {
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
			circleHazardDeletionList.push_back(ch->getGameID());
		}

		if (shouldBeKilled) {
			//shouldBeKilled = b->kill();
			//if (shouldBeKilled) {
				bulletDeletionList.push_back(b->getGameID());
				//it's possible to add the same bullet twice, but deleting by ID doesn't care about that
			//}
		}
	}

	//broad phase
	collisionList = PhysicsHandler::sweepAndPrune<Circle*, Rect*>(BulletManager::getBulletCollisionList(), HazardManager::getRectHazardCollisionList());

	//narrow phase and resolve collision
	std::vector<Game_ID> rectHazardDeletionList;
	for (int i = 0; i < collisionList.size(); i++) {
		std::pair<int, int> collisionPair = collisionList[i];
		Bullet* b = BulletManager::getBullet(collisionPair.first);
		bool shouldBeKilled = false;

		//rectangles:
		RectHazard* rh = HazardManager::getRectHazard(collisionPair.second);
		bool killRectHazard = false;
		bool modifiedRectHazardCollision = false;
		bool overridedRectHazardCollision = false;
		bool noMoreRectHazardCollisionSpecials = false;

		if (!b->canCollideWith(rh)) {
			continue;
		}
		if (CollisionHandler::partiallyCollided(b, rh)) {
			for (int k = 0; k < b->bulletPowers.size(); k++) {
				if (b->bulletPowers[k]->getModifiesCollisionWithRectHazard(rh)) {
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
			rectHazardDeletionList.push_back(rh->getGameID());
		}

		if (shouldBeKilled) {
			//shouldBeKilled = b->kill();
			//if (shouldBeKilled) {
				bulletDeletionList.push_back(b->getGameID());
				//it's possible to add the same bullet twice, but deleting by ID doesn't care about that
			//}
		}
	}

	//clear deaths
	for (int i = bulletDeletionList.size() - 1; i >= 0; i--) {
		BulletManager::deleteBulletByID(bulletDeletionList[i]);
	}
	for (int i = circleHazardDeletionList.size() - 1; i >= 0; i--) {
		HazardManager::deleteCircleHazardByID(circleHazardDeletionList[i]);
	}
	for (int i = rectHazardDeletionList.size() - 1; i >= 0; i--) {
		HazardManager::deleteRectHazardByID(rectHazardDeletionList[i]);
	}
}

void GameMainLoop::bulletToBullet() {
	//TODO: modernize (add default vs custom collision stuff)
	//broad phase
	std::vector<std::pair<int, int>> collisionList = PhysicsHandler::sweepAndPrune<Circle*>(BulletManager::getBulletCollisionList());
	//TODO: this returns a large list only to get destroyed when the function ends; any way to reduce this memory footprint?

	//narrow phase and resolve collision
	std::vector<Game_ID> bulletDeletionList;
	for (int i = 0; i < collisionList.size(); i++) {
		std::pair<int, int> collisionPair = collisionList[i];
		Bullet* b_outer = BulletManager::getBullet(collisionPair.first);
		bool b_outerShouldDie = false;

		Bullet* b_inner = BulletManager::getBullet(collisionPair.second);
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
					bulletDeletionList.push_back(b_inner->getGameID());
				}
			}
		}

		if (b_outerShouldDie) {
			b_outerShouldDie = b_outer->kill();
			if (b_outerShouldDie) {
				bulletDeletionList.push_back(b_outer->getGameID());
			}
		}
	}

	//clear deaths
	//std::sort(bulletDeletionList.begin(), bulletDeletionList.end());
	for (int i = bulletDeletionList.size() - 1; i >= 0; i--) {
		//BulletManager::deleteBullet(bulletDeletionList[i]);
		BulletManager::deleteBulletByID(bulletDeletionList[i]);
	}
}

void GameMainLoop::bulletToTank() {
	//TODO: tanks and bullets both have powers, so which one gets custom collision priority? (tanks, probably)
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b = BulletManager::getBullet(i);
		bool shouldBeKilled = false;
		bool wasKilled = false;

		for (int j = 0; j < TankManager::getNumTanks(); j++) {
			Tank* t = TankManager::getTank(j);
			bool killTank = false;
			bool modifiedTankCollision = false;
			bool overridedTankCollision = false;
			bool noMoreTankCollisionSpecials = false;

			if (b->canCollideWith(t)) {
				for (int k = 0; k < b->bulletPowers.size(); k++) {
					if (b->bulletPowers[k]->modifiesCollisionWithTank) {
						if (b->bulletPowers[k]->modifiedCollisionWithTankCanOnlyWorkIndividually && modifiedTankCollision) {
							continue;
						}
						if (noMoreTankCollisionSpecials) {
							continue;
						}

						modifiedTankCollision = true;
						if (b->bulletPowers[k]->overridesCollisionWithTank) {
							overridedTankCollision = true;
						}
						if (!b->bulletPowers[k]->modifiedCollisionWithTankCanWorkWithOthers) {
							noMoreTankCollisionSpecials = true;
						}

						InteractionBoolHolder check_temp = b->bulletPowers[k]->modifiedCollisionWithTank(b, t);
						if (check_temp.shouldDie) {
							shouldBeKilled = true;
						}
						if (check_temp.otherShouldDie) {
							killTank = true;
						}
					}
				}

				if (!overridedTankCollision) {
					if (CollisionHandler::partiallyCollided(t, b)) {
						InteractionBoolHolder result = EndGameHandler::determineWinner(t, b);
						killTank = result.shouldDie;
						shouldBeKilled = result.otherShouldDie;

						//if (shouldBeKilled) {
							//shouldBeKilled = b->kill();
							//if (shouldBeKilled) {
								BulletManager::deleteBullet(i);
								wasKilled = true;
								break;
							//}
							//the bullet needs to be killed
						//}
					}
				}
			}

			if (killTank) {
				//TODO: proper implementation?
			}
		}

		if (shouldBeKilled && !wasKilled) {
			BulletManager::deleteBullet(i);
		}
	}
}

void GameMainLoop::drawMain() {
	GameMainLoop::currentlyDrawing = true;

	auto start = Diagnostics::getTime();

	Diagnostics::startTiming("clear");
	Renderer::BeginningStuff();
	Renderer::Clear();
	Diagnostics::endTiming();

	Diagnostics::startTiming("background rect");
	BackgroundRect::draw();
	Renderer::UnbindAll();
	Diagnostics::endTiming();


	//is this needed? //ehh it can stay, may be needed for emergency CPU drawings
	// Set up the transformations stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	Diagnostics::startTiming("hazards under");
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		HazardManager::getCircleHazard(i)->draw(DrawingLayers::under);
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		HazardManager::getRectHazard(i)->draw(DrawingLayers::under);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("level under");
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->draw(DrawingLayers::under);
	}
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->drawLevelEffects(DrawingLayers::under);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("powerups");
	for (int i = 0; i < PowerupManager::getNumPowerups(); i++) {
		PowerupManager::getPowerup(i)->draw(DrawingLayers::normal);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("hazards");
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		HazardManager::getCircleHazard(i)->draw(DrawingLayers::normal);
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		HazardManager::getRectHazard(i)->draw(DrawingLayers::normal);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("walls");
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		WallManager::getWall(i)->draw(DrawingLayers::normal);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("bullets");
	for (int i = 0; i < BulletManager::getNumBullets(); i++) {
		BulletManager::getBullet(i)->draw(DrawingLayers::normal);
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
		TankManager::getTank(i)->draw(DrawingLayers::normal);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("hazards effects");
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		HazardManager::getCircleHazard(i)->draw(DrawingLayers::effects);
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		HazardManager::getRectHazard(i)->draw(DrawingLayers::effects);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("level effects");
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->draw(DrawingLayers::effects);
	}
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->drawLevelEffects(DrawingLayers::effects);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("tanks dead");
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->draw(DrawingLayers::top);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	auto end = Diagnostics::getTime();
	Diagnostics::pushGraphTime("draw", Diagnostics::getDiff(start, end));
	Diagnostics::drawGraphTimes();
	Renderer::UnbindAll(); //needed

	Diagnostics::startTiming("flush");
	Renderer::Cleanup();

	//for single framebuffer, use glFlush; for double framebuffer, swap the buffers
	//swapping buffers is limited to monitor refresh rate, so I use glFlush
	glFlush();
	//glutSwapBuffers();
	Diagnostics::endTiming();

	//end = Diagnostics::getTime();

	//Diagnostics::printPreciseTimings();
	Diagnostics::clearTimes();

	//std::cout << "entire: " << (long long)Diagnostics::getDiff(start, end) << "ms" << std::endl << std::endl;

	currentlyDrawing = false;
}

void GameMainLoop::drawLayer(DrawingLayers layer) {
	GameMainLoop::currentlyDrawing = true;

	auto start = Diagnostics::getTime();

	Diagnostics::startTiming("powerups");
	for (int i = 0; i < PowerupManager::getNumPowerups(); i++) {
		PowerupManager::getPowerup(i)->draw(layer);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("hazards");
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		HazardManager::getCircleHazard(i)->draw(layer);
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		HazardManager::getRectHazard(i)->draw(layer);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("walls");
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		WallManager::getWall(i)->draw(layer);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("bullets");
	for (int i = 0; i < BulletManager::getNumBullets(); i++) {
		BulletManager::getBullet(i)->draw(layer);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("tanks");
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->draw(layer);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("level");
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->draw(layer);
	}
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->drawLevelEffects(layer);
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming("flush");
	Renderer::Cleanup();
	glFlush();
	//glutSwapBuffers();
	Diagnostics::endTiming();

	auto end = Diagnostics::getTime();

	//Diagnostics::printPreciseTimings();
	Diagnostics::clearTimes();

	//std::cout << "entire: " << (long long)Diagnostics::getDiff(start, end) << "ms" << std::endl << std::endl;

	currentlyDrawing = false;
}

void GameMainLoop::drawAllLayers() {
	Renderer::BeginningStuff();
	Renderer::Clear();
	BackgroundRect::draw();
	Renderer::UnbindAll();
	/*
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	*/

	drawLayer(DrawingLayers::under);
	drawLayer(DrawingLayers::normal);
	drawLayer(DrawingLayers::effects);
	drawLayer(DrawingLayers::top);
	//drawLayer(DrawingLayers::debug);
}
