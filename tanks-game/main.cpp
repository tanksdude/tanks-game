#pragma once
#include <iostream>
#include <vector>
#include <time.h>
#include <unordered_map>

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
#include "cannonpoint.h"
#include "wall.h"
#include "bullet.h"
#include "powersquare.h"
#include "level.h"
#include "hazard.h"
#include "circlehazard.h"
#include "recthazard.h"

//managers:
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
#include "bulletpriorityhandler.h"
#include "colormixer.h"
#include "powerfunctionhelper.h"

//levels:
#include "defaultrandomlevel.h"
#include "developerlevel0.h"
#include "emptylevel.h"
#include "corridorlevel.h"
#include "bigfunlevel.h"

//hazards:
#include "stationaryturret.h"

//powers:
#include "inheritedpowercommon.h"
#include "powersquare.h"
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

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "diagnostics.h"

using namespace std;

int tank_dead = 0;

long frameCount = 0; //doesn't need a long for how it's interpreted...
long ticksUntilFrame = 1; //whatever again
long trueFrameCount = 0;
int physicsRate = 100; //(in Hz)
bool currentlyDrawing = false; //look into std::mutex

bool leftMouse = false;
bool rightMouse = false;

void doThing() {
	return;
}

int width = 1200*1.25;
int height = 600*1.25;

void appDrawScene() {
	currentlyDrawing = true;

	auto start = Diagnostics::getTime();

	Diagnostics::startTiming();
	Diagnostics::addName("clear");

	Renderer::Clear();

	Diagnostics::endTiming();
	
	Diagnostics::startTiming();
	Diagnostics::addName("background rect");

	BackgroundRect::draw();
	Renderer::UnbindAll(); //I honestly don't know if this is needed anymore but it doesn't hurt performance too much so it can stay
	
	Diagnostics::endTiming();
	

	//is this needed? //ehh it can stay, may be needed for emergency CPU drawings
	// Set up the transformations stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	Diagnostics::startTiming();
	Diagnostics::addName("powerups");
	for (int i = 0; i < PowerupManager::getNumPowerups(); i++) {
		PowerupManager::getPowerup(i)->draw();
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming();
	Diagnostics::addName("hazards");
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		HazardManager::getCircleHazard(i)->draw();
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		HazardManager::getRectHazard(i)->draw();
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming();
	Diagnostics::addName("walls");
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		WallManager::getWall(i)->draw();
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();
	
	Diagnostics::startTiming();
	Diagnostics::addName("bullets");
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

	Diagnostics::startTiming();
	Diagnostics::addName("tanks");
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->draw();
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming();
	Diagnostics::addName("flush");

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

// Handles window resizing
void appReshapeFunc(int w, int h) {
	width = w;
	height = h;

	double scale, center;
	double winXmin, winXmax, winYmin, winYmax;

	// Define x-axis and y-axis range (for CPU)
	const double appXmin = 0.0;
	const double appXmax = GAME_WIDTH;
	const double appYmin = 0.0;
	const double appYmax = GAME_HEIGHT;

	// Define that OpenGL should use the whole window for rendering (on CPU)
	glViewport(0, 0, w, h);

	// Set up the projection matrix using a orthographic projection that will
	// maintain the aspect ratio of the scene no matter the aspect ratio of
	// the window, and also set the min/max coordinates to be the disired ones (CPU only)
	w = (w == 0) ? 1 : w;
	h = (h == 0) ? 1 : h;

	if ((appXmax - appXmin) / w < (appYmax - appYmin) / h) { //too wide
		scale = ((appYmax - appYmin) / h) / ((appXmax - appXmin) / w);
		center = 0;
		winXmin = center - (center - appXmin) * scale;
		winXmax = center + (appXmax - center) * scale;
		winYmin = appYmin;
		winYmax = appYmax;
		Renderer::proj = glm::ortho(0.0f, float(GAME_WIDTH*scale), 0.0f, (float)GAME_HEIGHT); //GPU
	}
	else { //too tall
		scale = ((appXmax - appXmin) / w) / ((appYmax - appYmin) / h);
		center = 0;
		winYmin = center - (center - appYmin) * scale;
		winYmax = center + (appYmax - center) * scale;
		winXmin = appXmin;
		winXmax = appXmax;
		Renderer::proj = glm::ortho(0.0f, (float)GAME_WIDTH, 0.0f, float(GAME_HEIGHT*scale)); //GPU
	}

	// Now we use glOrtho to set up our viewing frustum (CPU only)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(winXmin, winXmax, winYmin, winYmax, -1, 1);
}

void appMotionFunc(int x, int y) {
	//dev tools
	if (leftMouse) {
		if (!rightMouse) { //tank 1
			TankManager::getTank(0)->giveX() = (x / double(width)) * GAME_WIDTH;
			TankManager::getTank(0)->giveY() = (1 - y / double(height)) * GAME_HEIGHT;
		}
		else { //tank 2
			TankManager::getTank(1)->giveX() = (x / double(width)) * GAME_WIDTH;
			TankManager::getTank(1)->giveY() = (1 - y / double(height)) * GAME_HEIGHT;
		}
	}
	//positions are off when window aspect ratio isn't 2:1
}

void mouse_func(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			leftMouse = true;
		} else if (button == GLUT_RIGHT_BUTTON) {
			rightMouse = !rightMouse;
		}
	} else {
		if (button == GLUT_LEFT_BUTTON) {
			leftMouse = false;
		}
	}
}

void mousewheel_func(int wheel, int dir, int x, int y) {
	// in the future, the wheel should change the index of some list of stuffs as a dev menu to insert said stuffs
	int real_x = (x / double(width)) * GAME_WIDTH;
	int real_y = (1 - y / double(height)) * GAME_HEIGHT;

	if (dir == 1) { //scroll up
		std::string* powers = new std::string[2]{ "homing", "bounce" };
		PowerupManager::pushPowerup(new PowerSquare(real_x, real_y, powers, 2));
		delete[] powers;
	} else { //scroll down
		HazardManager::pushCircleHazard(new StationaryTurret(real_x, real_y, 0));
	}

	//cout << "wheel:" << wheel << ", dir:" << dir << endl;
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

	//move tanks:
	moveTanks();

	//collide tanks with powerups:
	Diagnostics::startTiming();
	Diagnostics::addName("tank to powerups");
	tankToPowerup();
	Diagnostics::endTiming();
	
	//tick hazards:
	tickHazards();
	
	//move bullets:
	moveBullets();

	//powerCalculate on tanks and bullets, then tank shoot:
	Diagnostics::startTiming();
	Diagnostics::addName("powerCalculate and tank shoot");
	tankPowerCalculate();
	bulletPowerCalculate();
	tankShoot();
	//cout << BulletManager::getNumBullets() << endl;
	Diagnostics::endTiming();

	//collide tanks with walls:
	Diagnostics::startTiming();
	Diagnostics::addName("tank to walls");
	tankToWall();
	Diagnostics::endTiming();

	//collide tanks with hazards:
	Diagnostics::startTiming();
	Diagnostics::addName("tank-hazards");
	tankToHazard();
	Diagnostics::endTiming();

	//collide tanks with tanks:
	Diagnostics::startTiming();
	Diagnostics::addName("tank-tank");
	tankToTank();
	Diagnostics::endTiming();

	//collide tanks against edges:
	tankToEdge();
	
	//collide bullets against edges:
	Diagnostics::startTiming();
	Diagnostics::addName("bullet-edge");
	bulletToEdge();
	Diagnostics::endTiming();

	//collide bullets with walls:
	Diagnostics::startTiming();
	Diagnostics::addName("bullet-wall");
	bulletToWall();
	Diagnostics::endTiming();
	//bullet to wall is a big timesink, but it can only really be sped up by multithreading

	//collide bullets with hazards:
	Diagnostics::startTiming();
	Diagnostics::addName("bullet-hazards");
	bulletToHazard();
	Diagnostics::endTiming();
	//probably another big timesink, but there aren't many hazards

	//collide bullets with bullets:
	Diagnostics::startTiming();
	Diagnostics::addName("bullet-bullet");
	bulletToBullet();
	Diagnostics::endTiming();
	//add another shader: main uses proj, modify doesn't
	//bullet to bullet collision is the biggest timesink (obviously)
	//unfortunately it can only be O(n^2), and multithreading doesn't seem like it would work

	//collide bullets with tanks:
	Diagnostics::startTiming();
	Diagnostics::addName("bullet-tank");
	bulletToTank();
	Diagnostics::endTiming();

	//edge constrain tanks again in case bullet decided to move tank
	//don't edge constrain if said tank is dead //fix: implement
	/*
	tankToEdge();
	*/

	auto end = Diagnostics::getTime();

	//Diagnostics::printPreciseTimings();
	Diagnostics::clearTimes();

	//std::cout << "tick: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << endl << endl;

	trueFrameCount++;
	if (tank_dead == 0) {
		glutTimerFunc(1000/physicsUPS, tick, physicsUPS);
		if (frameCount == 0) {
			glutPostRedisplay();
		}
		++frameCount %= ticksUntilFrame;
	} else {
		tank_dead = 0;
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
	for (int i = 0; i < BulletManager::getNumBullets(); i++) {
		BulletManager::getBullet(i)->move();
	}
}

void tankPowerCalculate() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->powerCalculate();
	}
}

void bulletPowerCalculate() {
	for (int i = 0; i < BulletManager::getNumBullets(); i++) {
		Bullet* b = BulletManager::getBullet(i);
		b->powerCalculate();
		if (b->isDead()) {
			BulletManager::deleteBullet(i);
			i--;
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
		bool shouldBeKilled = false; //maybe the walls are poison with a certain powerup? I dunno, but gotta have it as an option
		Tank* t = TankManager::getTank(i);

		for (int j = 0; j < WallManager::getNumWalls(); j++) {
			bool modifiedWallCollision = false;
			bool overridedWallCollision = false;
			bool noMoreWallCollisionSpecials = false;
			bool killWall = false;
			Wall* w = WallManager::getWall(j);

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

						PowerInteractionBoolHolder check_temp = t->tankPowers[k]->modifiedCollisionWithWall(t, w);
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
				j--;
			}
		}

		if (shouldBeKilled) {
			tank_dead = 1; //TODO: proper implementation
		}

	}
}

void tankToHazard() {
	//temporary!
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		Tank* t = TankManager::getTank(i);
		//circles:
		for (int j = 0; j < HazardManager::getNumCircleHazards(); j++) {
			CircleHazard* ch = HazardManager::getCircleHazard(j);
			if (CollisionHandler::partiallyCollided(t, ch)) {
				CollisionHandler::pushMovableAwayFromImmovable(t, ch);
			}
		}
		//rectangles:
		for (int j = 0; j < HazardManager::getNumRectHazards(); j++) {
			RectHazard* rh = HazardManager::getRectHazard(j);
			if (CollisionHandler::partiallyCollided(t, rh)) {
				CollisionHandler::pushMovableAwayFromImmovable(t, rh);
			}
		}
	}
}

void tankToTank() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		bool shouldBeKilled = false;
		Tank* t1 = TankManager::getTank(i);

		for (int j = 0; j < TankManager::getNumTanks(); j++) {
			if (i == j) {
				continue;
			}

			bool modifiedTankCollision = false;
			bool overridedTankCollision = false;
			bool noMoreTankCollisionSpecials = false;
			bool killOtherTank = false;
			Tank* t2 = TankManager::getTank(j);

			if (CollisionHandler::partiallyCollided(t1, t2)) {
				for (int k = 0; k < t1->tankPowers.size(); k++) {
					if (t1->tankPowers[k]->modifiesCollisionWithWall) {
						if (t1->tankPowers[k]->modifiedCollisionWithTankCanOnlyWorkIndividually && modifiedTankCollision) {
							continue;
						}
						if (noMoreTankCollisionSpecials) {
							continue;
						}

						modifiedTankCollision = true;
						if (t1->tankPowers[k]->overridesCollisionWithTank) {
							overridedTankCollision = true;
						}
						if (!t1->tankPowers[k]->modifiedCollisionWithTankCanWorkWithOthers) {
							noMoreTankCollisionSpecials = true;
						}

						PowerInteractionBoolHolder check_temp = t1->tankPowers[k]->modifiedCollisionWithTank(t1, t2);
						if (check_temp.shouldDie) {
							shouldBeKilled = true;
						}
						if (check_temp.otherShouldDie) {
							if (t1->getID() != t2->getID()) {
								killOtherTank = true;
							}
						}
					}
				}

				if (!overridedTankCollision) {
					CollisionHandler::pushMovableAwayFromMovable(t1, t2);
				}
			}

			if (killOtherTank) {
				tank_dead = 1;
			}
		}

		if (shouldBeKilled) {
			tank_dead = 1;
		}
	}
}

void tankToEdge() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		bool shouldBeKilled = false;

		bool modifiedEdgeCollision = false;
		bool overridedEdgeCollision = false;
		bool noMoreEdgeCollisionSpecials = false;
		Tank* t = TankManager::getTank(i);

		if (t->isPartiallyOutOfBounds()) {
			for (int k = 0; k < t->tankPowers.size(); k++) {
				if (t->tankPowers[k]->modifiesCollisionWithEdge) {
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

					PowerInteractionBoolHolder check_temp = t->tankPowers[k]->modifiedEdgeCollision(t);
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
			tank_dead = 1;
		}
	}
}

void bulletToEdge() {
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		bool modifiedEdgeCollision = false;
		bool overridedEdgeCollision = false;
		bool noMoreEdgeCollisionSpecials = false;
		bool shouldBeKilled = false;
		Bullet* b = BulletManager::getBullet(i);

		if (b->isPartiallyOutOfBounds()) {
			for (int k = 0; k < b->bulletPowers.size(); k++) {
				if (b->bulletPowers[k]->modifiesCollisionWithEdge) {
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

					PowerInteractionBoolHolder check_temp = b->bulletPowers[k]->modifiedEdgeCollision(b);
					if (check_temp.shouldDie) {
						shouldBeKilled = true;
					}
				}
			}

			if (overridedEdgeCollision) {
				continue;
			}
			if (b->isFullyOutOfBounds()) {
				shouldBeKilled = true;
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
		bool shouldBeKilled = false;
		Bullet* b = BulletManager::getBullet(i);

		for (int j = WallManager::getNumWalls() - 1; j >= 0; j--) {
			bool modifiedWallCollision = false;
			bool overridedWallCollision = false;
			bool noMoreWallCollisionSpecials = false;
			bool killWall = false;
			Wall* w = WallManager::getWall(j);

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

						PowerInteractionBoolHolder check_temp = b->bulletPowers[k]->modifiedCollisionWithWall(b, w);
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
	//temporary!
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b = BulletManager::getBullet(i);

		bool bullet_died = false;
		//circles:
		for (int j = 0; j < HazardManager::getNumCircleHazards(); j++) {
			if (CollisionHandler::partiallyCollided(b, HazardManager::getCircleHazard(j))) {
				BulletManager::deleteBullet(i);
				bullet_died = true;
				break;
			}
		}
		if (bullet_died) {
			continue;
		}
		//rectangles:
		for (int j = 0; j < HazardManager::getNumRectHazards(); j++) {
			if (CollisionHandler::partiallyCollided(b, HazardManager::getRectHazard(j))) {
				BulletManager::deleteBullet(i);
				break;
			}
		}
	}
}

void bulletToBullet() {
	//TODO: modernize (add default vs custom collision stuff)
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b_outer = BulletManager::getBullet(i);

		for (int j = BulletManager::getNumBullets() - 1; j >= 0; j--) { //could start at i-1? //fix: find out
			Bullet* b_inner = BulletManager::getBullet(j);

			if (b_outer->getID() == b_inner->getID()) {
				continue;
			}
			if (CollisionHandler::partiallyCollided(b_outer, b_inner)) {
				char result = BulletPriorityHandler::determinePriority(b_outer, b_inner);
				if (result <= -2) {
					bool firstDies = rand()%2;
					if (firstDies) {
						BulletManager::deleteBullet(i);
						break;
					} else {
						BulletManager::deleteBullet(j);
						continue; //fix: is this supposed to be break?
					}
				} else if (result == -1) { //both die
					if (i > j) {
						BulletManager::deleteBullet(i);
						BulletManager::deleteBullet(j);
						i--;
					} else {
						BulletManager::deleteBullet(j);
						BulletManager::deleteBullet(i);
					}
					break;
				} else if (result >= 2) { //it's a draw, so neither dies
					//continue;
				} else {
					if (result == 0) {
						BulletManager::deleteBullet(i);
						i--;
						break;
					} else {
						BulletManager::deleteBullet(j);
						if (i > j) {
							i--;
						}
						continue; //not needed //shouldn't be break because a single bullet may be capable of destroying multiple other bullets (invincibility stuff)
					}
				}

				//break;
			}
		}
	}
}

void bulletToTank() {
	//TODO: modernize (add default vs custom collision stuff)
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		Tank* t = TankManager::getTank(i);

		for (int j = 0; j < BulletManager::getNumBullets(); j++) {
			Bullet* b = BulletManager::getBullet(j);

			if (b->getID() == t->getID()) {
				continue;
			}
			if (CollisionHandler::partiallyCollided(t, b)) {
				char result = BulletPriorityHandler::determinePriority(b, t);
				if (result <= -2) {
					bool firstDies = rand()%2;
					if (firstDies) {
						tank_dead = 1;
						continue;
					} else {
						BulletManager::deleteBullet(j);
						j--;
						continue; //fix: is this supposed to be break?
					}
				} else if (result == -1) { //both die
					tank_dead = 1;

					/*
					BulletManager::deleteBullet(j);
					j--;
					*/

					continue;
				} else if (result >= 2) { //it's a draw, so neither dies (probably not going to happen)
					//continue;
				} else {
					if (result == 0) {
						BulletManager::deleteBullet(j);
						j--;
						continue; //not needed //fix: should it be break?
					} else {
						tank_dead = 1;
						continue;
					}
				}

				//break;
			}
		}
	}
}



int main(int argc, char** argv) {
	srand(time(NULL));

	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	//thanks to https://community.khronos.org/t/wglmakecurrent-issues/62656/3 for solving why a draw call would take ~15ms for no reason

	// Setup window position, size, and title
	glutInitWindowPosition(60, 60);
	glutInitWindowSize(width, height);
	glutCreateWindow("Tanks Test v0.2.1 NOT FINAL"); //this is not guranteed to be correct every commit but likely will be

	// Setup some OpenGL options
	glPointSize(2);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//initialize glew
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

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

	LevelManager::addLevelToHashmap(new DeveloperLevel0());
	LevelManager::addLevelToHashmap(new DefaultRandomLevel());
	LevelManager::addLevelToHashmap(new EmptyLevel());
	LevelManager::addLevelToHashmap(new CorridorLevel());
	LevelManager::addLevelToHashmap(new BigFunLevel());

	HazardManager::addCircleHazardFactory(StationaryTurret::factory);

	//initialize managers:
	KeypressManager::initialize();
	BulletManager::initialize();
	PowerupManager::initialize();
	WallManager::initialize();
	LevelManager::initialize();
	HazardManager::initialize();
	
	//static VAO, VBO, and IBO has better performance
	Renderer::Initialize();
	BackgroundRect::initializeGPU();
	//Tank::initializeGPU();
	//Bullet::initializeGPU();
	//PowerSquare::initializeGPU();
	//Wall::initializeGPU();
	//RectHazard::initializeGPU();
	//CircleHazard::initializeGPU();


	// Set callback for drawing the scene
	glutDisplayFunc(appDrawScene);

	// Set callback for resizing the window
	glutReshapeFunc(appReshapeFunc);

	//mouse clicking
	glutMouseFunc(mouse_func);

	// Set callback to handle mouse dragging
	glutMotionFunc(appMotionFunc);

	// Set callback to handle keyboard events
	glutKeyboardFunc(KeypressManager::setNormalKey);

	//callback for keyboard up events
	glutKeyboardUpFunc(KeypressManager::unsetNormalKey);

	//special keyboard down
	glutSpecialFunc(KeypressManager::setSpecialKey);

	//special keyboard up
	glutSpecialUpFunc(KeypressManager::unsetSpecialKey);

	//mousewheel
	glutMouseWheelFunc(mousewheel_func);

	// Set callback for the idle function
	//glutIdleFunc(draw);

	//main game code initialization stuff:
	TankManager::pushTank(new Tank(20, 160, 0, 0, "WASD", { false, 'w' }, { false, 'a' }, { false, 'd' }, { false, 's' }));
	TankManager::pushTank(new Tank(620, 160, PI, 1, "Arrow Keys", { true, GLUT_KEY_UP }, { true, GLUT_KEY_LEFT }, { true, GLUT_KEY_RIGHT }, { true, GLUT_KEY_DOWN }));
#if _DEBUG
	LevelManager::getLevelByName("dev0")->initialize();
#else
	LevelManager::getLevelByName("default random")->initialize();
#endif
	/*
	for (int i = 0; i < 4; i++) {
		walls.push_back(new Wall(320 - 240*(((3-i)/2) * 2 - 1) - 32*((((3-i)/2) + 1) % 2), i%2 * (320-128), 32, 128, ColorValueHolder(255,0,255)));
	}
	*/

	//framelimiter
	glutTimerFunc(1000/physicsRate, tick, physicsRate);

	cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << endl;
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;

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
 * 90% theoretical foundation: no hazard powers, no level... anything
 * 70% actual foundation: not every "modification function" actually does something in the main
 * 25% game code:
 * * first off, don't know what will be final beyond the ideas located in power.h and elsewhere
 * * second, it's a complete estimate (obviously) and this is a restatement of the first
 * * third, 100% probably won't be "finished" on this scale (restatement of the second?)
 * * fourth, percentage is horribly imprecise because, like most people, I think about completion percentages on personal projects in 5% increments (restatement of third)
 * * fifth, here's what's next:
 * * * new powerups
 * * * new levels
 * * * more hazards
 */
