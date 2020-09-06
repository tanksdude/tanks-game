#pragma once
#include <iostream>
#include <vector>
#include <time.h>
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

//levels:
#include "defaultrandomlevel.h"
#include "emptylevel.h"
#include "corridorlevel.h"
#include "bigfunlevel.h"
#include "fewobstacleslevel.h"
#include "concealedpowerupslevel.h"
#include "manyhazardslevel.h"
//"extra" (is there even a reason for this?):
#include "hidingplaceslevel.h"
//dev levels:
#include "developerlevel0.h"
#include "developerlevel1.h"
#include "developerlevel2.h"
//old levels:
#include "oldemptylevel.h"

//hazards:
#include "stationaryturret.h"
#include "targetingturret.h"
#include "rectangularlightning.h"
#include "horizontallightning.h"
#include "verticallightning.h"
#include "circularlightning.h"
#include "rectangularlava.h"
#include "circularlava.h"
#include "rectangularnobulletzone.h"
#include "circularnobulletzone.h"

//powers:
#include "speedpower.h"
#include "wallhackpower.h"
#include "multishotpower.h"
#include "bouncepower.h"
#include "triplepower.h"
#include "homingpower.h"
#include "invinciblenamedpower.h"
#include "bignamedpower.h"
#include "megadeathpower.h"
#include "grenadepower.h"
#include "firenamedpower.h"
#include "blastpower.h"
#include "bananapower.h"
#include "godmodepower.h"
//"extra":
#include "minespower.h"
#include "shotgunpower.h"
#include "trackingpower.h"
//dev powers:
#include "devlonginvinciblenamedpower.h" //invincible but lasts a long time
#include "inversionpower.h" //flips left and right turning
//old powers:
#include "oldbouncepower.h" //identical to bounce except it makes the tank smaller
#include "oldbignamedpower.h" //big but moves at half speed (not quarter) and the speed stacks

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "diagnostics.h"

using namespace std;

//tank team rules:
//0  = no team or default team
//-1 = hazard team (hazards can be split up into multiple teams, but by default there's only one)
//1  = default teamID of tank #1 ("WASD")
//2  = default teamID of tank #2 ("Arrow Keys")
//tank teams always > 0

long frameCount = 0; //doesn't need a long for how it's interpreted...
long ticksUntilFrame = 1; //whatever again
int physicsRate = 100; //(in Hz)
bool currentlyDrawing = false; //look into std::mutex

void doThing() {
	return;
}

void appDrawScene() {
	currentlyDrawing = true;

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

//tick stuff:
void moveTanks();
void tankToPowerup();
void tickHazards();
void moveBullets();
void tankPowerCalculate();
void bulletPowerCalculate();
void tankShoot();
void tankToWall();
void tankToHazard();
void tankToTank();
void tankToEdge();
void bulletToEdge();
void bulletToWall();
void bulletToHazard();
void bulletToBullet();
void bulletToTank();

void tick(int physicsUPS) {
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
	//don't edge constrain if said tank is dead //fix: implement
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
		glutTimerFunc(1000/physicsUPS, tick, physicsUPS);
		if (frameCount == 0) {
			glutPostRedisplay();
		}
		++frameCount %= ticksUntilFrame;
	} else {
		glutPostRedisplay();
		glutTimerFunc(1000, ResetThings::reset, 0);
		glutTimerFunc(1000 + 1000/physicsUPS, tick, physicsUPS);
	}
}
void tick() { tick(physicsRate); }
void draw() { glutPostRedisplay(); }

void moveTanks() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->move();
	}
}

void tankToPowerup() {
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

void tickHazards() {
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		HazardManager::getCircleHazard(i)->tick();
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		HazardManager::getRectHazard(i)->tick();
	}
}

void moveBullets() {
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

void tankPowerCalculate() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->powerCalculate();
	}
}

void bulletPowerCalculate() {
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b = BulletManager::getBullet(i);
		b->powerCalculate();
		if (b->isDead()) {
			BulletManager::deleteBullet(i);
			continue;
		}
	}
}

void tankShoot() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->shoot();
	}
}

void tankToWall() {
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

void tankToHazard() {
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

void tankToTank() {
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

void tankToEdge() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		Tank* t = TankManager::getTank(i);
		bool shouldBeKilled = false;
		bool modifiedEdgeCollision = false;
		bool overridedEdgeCollision = false;
		bool noMoreEdgeCollisionSpecials = false;

		if (t->isPartiallyOutOfBounds()) {
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
				t->edgeConstrain();
			}
		}

		if (shouldBeKilled) {
			//TODO: proper implementation?
		}
	}
}

void bulletToEdge() {
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b = BulletManager::getBullet(i);
		bool shouldBeKilled = false;
		bool modifiedEdgeCollision = false;
		bool overridedEdgeCollision = false;
		bool noMoreEdgeCollisionSpecials = false;

		if (b->isPartiallyOutOfBounds()) {
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
				if (b->isFullyOutOfBounds()) {
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

void bulletToWall() {
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
			BulletManager::deleteBullet(i);
		}
	}
}

void bulletToHazard() {
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
			BulletManager::deleteBullet(i);
			continue; //bullet died, can't do any more collision with it
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
			BulletManager::deleteBullet(i);
		}
	}
}

void bulletToBullet() {
	//TODO: modernize (add default vs custom collision stuff)
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b_outer = BulletManager::getBullet(i);
		bool b_outerShouldDie = false;

		for (int j = BulletManager::getNumBullets() - 1; j >= 0; j--) { //could start at i-1? //fix: find out
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
				b_outerShouldDie = result.shouldDie;
				b_innerShouldDie = result.otherShouldDie;
			}

			if (b_innerShouldDie) {
				BulletManager::deleteBullet(j);
				if (i > j) {
					i--;
				}
			}
		}

		if (b_outerShouldDie) {
			BulletManager::deleteBullet(i);
		}
	}
}

void bulletToTank() {
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
			}

			if (killBullet) {
				BulletManager::deleteBullet(j);
			}
		}

		if (shouldBeKilled) {
			//TODO: proper implementation?
		}
	}
}



int main(int argc, char** argv) {
	srand(time(NULL));

	Renderer::SetContext(AvailableRenderingContexts::OpenGL);
	try {
		Renderer::PreInitialize(&argc, argv, "PowerTanks Battle v0.2.4 NOT FINAL"); //this is not guaranteed to be correct every commit but likely will be
	}
	catch (exception& e) {
		cout << e.what() << endl;
		return 1;
	}
	
	// Set callback for drawing the scene
	glutDisplayFunc(appDrawScene);

	// Set callback for resizing the window
	glutReshapeFunc(Renderer::windowResizeFunc);

	//mouse clicking
	glutMouseFunc(DeveloperManager::mouseClickFunc);

	// Set callback to handle mouse dragging
	glutMotionFunc(DeveloperManager::mouseDragFunc);

	// Set callback to handle keyboard events
	glutKeyboardFunc(KeypressManager::setNormalKey);

	//callback for keyboard up events
	glutKeyboardUpFunc(KeypressManager::unsetNormalKey);

	//special keyboard down
	glutSpecialFunc(KeypressManager::setSpecialKey);

	//special keyboard up
	glutSpecialUpFunc(KeypressManager::unsetSpecialKey);

	//mousewheel
	glutMouseWheelFunc(DeveloperManager::mouseWheelFunc);

	// Set callback for the idle function
	//glutIdleFunc(draw);
	
	//vanilla (should some be "old"?):
	PowerupManager::addPowerFactory(SpeedPower::factory);
	PowerupManager::addPowerFactory(WallhackPower::factory);
	PowerupManager::addPowerFactory(BouncePower::factory);
	PowerupManager::addPowerFactory(MultishotPower::factory);
	PowerupManager::addPowerFactory(TriplePower::factory);
	PowerupManager::addPowerFactory(HomingPower::factory);
	PowerupManager::addPowerFactory(InvincibleNamedPower::factory);
	PowerupManager::addPowerFactory(BigNamedPower::factory);
	PowerupManager::addPowerFactory(MegaDeathPower::factory);
	PowerupManager::addPowerFactory(GrenadePower::factory);
	PowerupManager::addPowerFactory(FireNamedPower::factory);
	PowerupManager::addPowerFactory(BlastPower::factory);
	PowerupManager::addPowerFactory(BananaPower::factory);
	PowerupManager::addPowerFactory(GodmodePower::factory);
	//"extra":
	PowerupManager::addPowerFactory(MinesPower::factory);
	PowerupManager::addPowerFactory(ShotgunPower::factory);
	PowerupManager::addPowerFactory(TrackingPower::factory);

	//dev:
	PowerupManager::addPowerFactory(DevLongInvincibleNamedPower::factory);
	PowerupManager::addPowerFactory(InversionPower::factory);

	//old:
	PowerupManager::addPowerFactory(OldBouncePower::factory);
	PowerupManager::addPowerFactory(OldBigNamedPower::factory);

	//vanilla (some are also "old"):
	LevelManager::addLevelFactory(DefaultRandomLevel::factory);
	LevelManager::addLevelFactory(EmptyLevel::factory);
	LevelManager::addLevelFactory(CorridorLevel::factory);
	LevelManager::addLevelFactory(BigFunLevel::factory);
	LevelManager::addLevelFactory(FewObstaclesLevel::factory);
	LevelManager::addLevelFactory(ConcealedPowerupsLevel::factory);
	LevelManager::addLevelFactory(ManyHazardsLevel::factory);
	//"extra":
	LevelManager::addLevelFactory(HidingPlacesLevel::factory);

	//dev:
	LevelManager::addLevelFactory(DeveloperLevel0::factory);
	LevelManager::addLevelFactory(DeveloperLevel1::factory);
	LevelManager::addLevelFactory(DeveloperLevel2::factory);
	
	//old:
	LevelManager::addLevelFactory(OldEmptyLevel::factory);

	HazardManager::addCircleHazardFactory(StationaryTurret::factory, StationaryTurret::randomizingFactory);
	HazardManager::addCircleHazardFactory(TargetingTurret::factory, TargetingTurret::randomizingFactory);
	HazardManager::addRectHazardFactory(RectangularLightning::factory, RectangularLightning::randomizingFactory);
	HazardManager::addRectHazardFactory(HorizontalLightning::factory, HorizontalLightning::randomizingFactory);
	HazardManager::addRectHazardFactory(VerticalLightning::factory, VerticalLightning::randomizingFactory);
	HazardManager::addCircleHazardFactory(CircularLightning::factory, CircularLightning::randomizingFactory);
	HazardManager::addRectHazardFactory(RectangularLava::factory, RectangularLava::randomizingFactory);
	HazardManager::addCircleHazardFactory(CircularLava::factory, CircularLava::randomizingFactory);
	HazardManager::addRectHazardFactory(RectangularNoBulletZone::factory, RectangularNoBulletZone::randomizingFactory);
	HazardManager::addCircleHazardFactory(CircularNoBulletZone::factory, CircularNoBulletZone::randomizingFactory);

	//initialize managers and stuff:
	GameManager::Initialize();
	KeypressManager::Initialize();
	BulletManager::initialize();
	PowerupManager::initialize();
	WallManager::initialize();
	LevelManager::initialize();
	HazardManager::initialize();
	Renderer::Initialize(); //static VAO, VBO, and IBO has better performance
	BackgroundRect::initializeGPU();

	//main game code initialization stuff:
	TankManager::pushTank(new Tank(20, GAME_HEIGHT/2, 0, 1, "WASD", { false, 'w' }, { false, 'a' }, { false, 'd' }, { false, 's' }, { false, 'e' }));
	TankManager::pushTank(new Tank(GAME_WIDTH-20, GAME_HEIGHT/2, PI, 2, "Arrow Keys", { true, GLUT_KEY_UP }, { true, GLUT_KEY_LEFT }, { true, GLUT_KEY_RIGHT }, { true, GLUT_KEY_DOWN }, { false, '/' }));
	EndGameHandler::addTeamToWatch(1, "\"WASD 4 Life\"");
	EndGameHandler::addTeamToWatch(2, "\"Arrow Keys R WINZ\"");
	//these are just the kind of names that will make me cringe in a month
	EndGameHandler::addTeamToWatch(HAZARD_TEAM, "HAZARDS");
#if _DEBUG
	LevelManager::pushLevel("dev", "dev2");
#else
	LevelManager::pushLevel("vanilla", "default random");
#endif
	ResetThings::firstReset();

	cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << endl;
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;

	//framelimiter
	glutTimerFunc(1000/physicsRate, tick, physicsRate);

	// Start the main loop
	glutMainLoop();

}

/*
 * estimated total completion:
 * 100% required GPU drawing stuff!
 * 20% theoretical GPU stuff (may not attempt)
 * * add a gradient shader
 * * gotta learn how to do batching
 * * make things more efficient (way easier said than done, I suppose)
 * 92.5?% theoretical foundation: no hazard powers, no level... anything
 * * hazard powers: ~50%
 * * levels: ~75%; no level effects
 * 90% actual foundation: not every "modification function" actually does something
 * * almost everything needed is somewhere, so copy-and-paste will be the solution (unless I can figure out member function pointers)
 * ~40% game code:
 * * I don't know what will be final beyond the ideas located in power.cpp, hazard.cpp, level.cpp, and elsewhere
 * * it's just an estimate
 * * 100% probably won't be "finished" on this scale (it could jump from 60% to 100%)
 */
