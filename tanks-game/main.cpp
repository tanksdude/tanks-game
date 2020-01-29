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
#include "res/vendor/glm/glm.hpp" //this library is overkill but that's okay
#include "res/vendor/glm/gtc/matrix_transform.hpp"
#include "res/vendor/glm/gtx/transform.hpp"

//important stuff:
#include "colorvalueholder.h"
#include "backgroundrect.h"
#include "tank.h"
#include "cannonpoint.h"
#include "wall.h"
#include "bullet.h"
#include "powersquare.h"
#include "level.h"

//classes with important handling functions:
#include "collisionhandler.h"
#include "resetthings.h"
#include "bulletpriorityhandler.h"
#include "colormixer.h"
#include "powerfunctionhelper.h"

//levels:
#include "randomlevel.h"
#include "emptylevel.h"
#include "corridorlevel.h"

//powers:
#include "inheritedpowercommon.h"
#include "powersquare.h"
#include "speedtankpower.h"
#include "speedbulletpower.h"
#include "speedpower.h"
#include "wallhacktankpower.h"
#include "wallhackbulletpower.h"
#include "wallhackpower.h"
#include "multishottankpower.h"
#include "multishotbulletpower.h"
#include "multishotpower.h"
#include "bouncetankpower.h"
#include "bouncebulletpower.h"
#include "bouncepower.h"
#include "tripletankpower.h"
#include "triplebulletpower.h"
#include "triplepower.h"
#include "homingtankpower.h"
#include "homingbulletpower.h"
#include "homingpower.h"
#include "invinciblenamedtankpower.h"
#include "invinciblenamedbulletpower.h"
#include "invinciblenamedpower.h"
#include "bignamedtankpower.h"
#include "bignamedbulletpower.h"
#include "bignamedpower.h"
#include "megadeathtankpower.h"
#include "megadeathbulletpower.h"
#include "megadeathpower.h"
#include "grenadetankpower.h"
#include "grenadebulletpower.h"
#include "grenadepower.h"

#include <GL/glew.h>
#include <GL/freeglut.h>
//TODO: move glm to Dependencies instead of res/vendor?

#include "diagnostics.h"

using namespace std;

unordered_map<unsigned char, bool> normalKeyStates;
unordered_map<int, bool> specialKeyStates;

Tank* tank1 = new Tank(20, 160, 0, 0, "WASD");
Tank* tank2 = new Tank(620, 160, PI, 1, "Arrow Keys");
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
	for (int i = 0; i < powerups.size(); i++) {
		powerups[i]->draw();
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();

	Diagnostics::startTiming();
	Diagnostics::addName("walls");
	for (int i = 0; i < walls.size(); i++) {
		walls[i]->draw();
	}
	Renderer::UnbindAll();
	Diagnostics::endTiming();
	
	Diagnostics::startTiming();
	Diagnostics::addName("bullets");
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->draw();
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
	for (int i = 0; i < tanks.size(); i++) {
		tanks[i]->draw();
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

void windowToScene(float& x, float& y) {
	x = (2.0f * (x / float(width))) - 1.0f;
	y = 1.0f - (2.0f * (y / float(height)));
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
		proj = glm::ortho(0.0f, float(GAME_WIDTH*scale), 0.0f, (float)GAME_HEIGHT); //GPU
	}
	else { //too tall
		scale = ((appXmax - appXmin) / w) / ((appYmax - appYmin) / h);
		center = 0;
		winYmin = center - (center - appYmin) * scale;
		winYmax = center + (appYmax - center) * scale;
		winXmin = appXmin;
		winXmax = appXmax;
		proj = glm::ortho(0.0f, (float)GAME_WIDTH, 0.0f, float(GAME_HEIGHT*scale)); //GPU
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
			tanks[0]->giveX() = (x / double(width)) * GAME_WIDTH;
			tanks[0]->giveY() = (1 - y / double(height)) * GAME_HEIGHT;
		}
		else { //tank 2
			tanks[1]->giveX() = (x / double(width)) * GAME_WIDTH;
			tanks[1]->giveY() = (1 - y / double(height)) * GAME_HEIGHT;
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

void keyboard_down(unsigned char key, int x, int y) {
	normalKeyStates[key] = true;
}

void special_keyboard_down(int key, int x, int y) {
	specialKeyStates[key] = true;
}

void keyboard_up(unsigned char key, int x, int y) {
	normalKeyStates[key] = false;
}

void special_keyboard_up(int key, int x, int y) {
	specialKeyStates[key] = false;
}


void tick(int physicsUPS) {
	//while (currentlyDrawing) {}

	auto start = Diagnostics::getTime();

	Diagnostics::startTiming();
	Diagnostics::addName("bool movement");
	//temporary: need to figure out better implementation
	tanks[0]->forward = normalKeyStates['w'];
	tanks[0]->turnL = normalKeyStates['a'];
	tanks[0]->turnR = normalKeyStates['d'];
	tanks[0]->shooting = normalKeyStates['s'];
	//still temporary
	tanks[1]->forward = specialKeyStates[GLUT_KEY_UP];
	tanks[1]->turnL = specialKeyStates[GLUT_KEY_LEFT];
	tanks[1]->turnR = specialKeyStates[GLUT_KEY_RIGHT];
	tanks[1]->shooting = specialKeyStates[GLUT_KEY_DOWN];
	Diagnostics::endTiming();

	//TODO: each tick portion needs to go in its own separate function; that way a level can override some parts of it without having to rewrite everything

	Diagnostics::startTiming();
	Diagnostics::addName("move");
	//move everything
	for (int i = 0; i < tanks.size(); i++) {
		tanks[i]->move();
	}
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->move();
	}
	Diagnostics::endTiming();

	Diagnostics::startTiming();
	Diagnostics::addName("powerup-tank");
	//do the special things
	for (int i = powerups.size() - 1; i >= 0; i--) {
		for (int j = 0; j < tanks.size(); j++) {
			if (CollisionHandler::partiallyCollided(powerups[i], tanks[j])) {
				powerups[i]->givePower(tanks[j]);
				delete powerups[i];
				powerups.erase(powerups.begin() + i);
				break;
			}
		}
	}
	Diagnostics::endTiming();

	Diagnostics::startTiming();
	Diagnostics::addName("powerCalculate and shoot");
	for (int i = 0; i < tanks.size(); i++) {
		tanks[i]->powerCalculate();
	}
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->powerCalculate();
		if (bullets[i]->isDead()) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
			i--;
			continue;
		}
	}

	for (int i = 0; i < tanks.size(); i++) {
		tanks[i]->shoot();
	}
	Diagnostics::endTiming();

	Diagnostics::startTiming();
	Diagnostics::addName("tank-wall");
	//tank to wall collision:
	for (int i = 0; i < tanks.size(); i++) {
		bool shouldBeKilled = false; //maybe the walls are poison with a certain powerup? I dunno, but gotta have it as an option

		for (int j = 0; j < walls.size(); j++) {
			bool modifiedWallCollision = false;
			bool overridedWallCollision = false;
			bool noMoreWallCollisionSpecials = false;
			bool killWall = false;

			if (CollisionHandler::partiallyCollided(tanks[i], walls[j])) {
				for (int k = 0; k < tanks[i]->tankPowers.size(); k++) {
					if (tanks[i]->tankPowers[k]->modifiesCollisionWithWall) {
						if (tanks[i]->tankPowers[k]->modifiedCollisionWithWallCanOnlyWorkIndividually && modifiedWallCollision) {
							continue;
						}
						if (noMoreWallCollisionSpecials) {
							continue;
						}

						modifiedWallCollision = true;
						if (tanks[i]->tankPowers[k]->overridesCollisionWithWall) {
							overridedWallCollision = true;
						}
						if (!tanks[i]->tankPowers[k]->modifiedEdgeCollisionCanWorkWithOthers) {
							noMoreWallCollisionSpecials = true;
						}

						PowerInteractionBoolHolder check_temp = tanks[i]->tankPowers[k]->modifiedCollisionWithWall(tanks[i], walls[j]);
						if (check_temp.shouldDie) {
							shouldBeKilled = true;
						}
						if (check_temp.otherShouldDie) {
							killWall = true;
						}
					}
				}

				if (!overridedWallCollision) {
					CollisionHandler::pushMovableAwayFromImmovable(tanks[i], walls[j]);
				}
			}

			if (killWall) {
				walls.erase(walls.begin() + j);
				j--;
			}
		}

		if (shouldBeKilled) {
			tank_dead = 1; //TODO: proper implementation
		}

	}
	Diagnostics::endTiming();

	Diagnostics::startTiming();
	Diagnostics::addName("tank-tank");
	for (int i = 0; i < tanks.size(); i++) {
		bool shouldBeKilled = false;
		for (int j = 0; j < tanks.size(); j++) {
			if (i == j) {
				continue;
			}

			bool modifiedTankCollision = false;
			bool overridedTankCollision = false;
			bool noMoreTankCollisionSpecials = false;
			bool killOtherTank = false;

			if (CollisionHandler::partiallyCollided(tanks[i], tanks[j])) {
				for (int k = 0; k < tanks[i]->tankPowers.size(); k++) {
					if (tanks[i]->tankPowers[k]->modifiesCollisionWithWall) {
						if (tanks[i]->tankPowers[k]->modifiedCollisionWithTankCanOnlyWorkIndividually && modifiedTankCollision) {
							continue;
						}
						if (noMoreTankCollisionSpecials) {
							continue;
						}

						modifiedTankCollision = true;
						if (tanks[i]->tankPowers[k]->overridesCollisionWithTank) {
							overridedTankCollision = true;
						}
						if (!tanks[i]->tankPowers[k]->modifiedCollisionWithTankCanWorkWithOthers) {
							noMoreTankCollisionSpecials = true;
						}

						PowerInteractionBoolHolder check_temp = tanks[i]->tankPowers[k]->modifiedCollisionWithTank(tanks[i], tanks[j]);
						if (check_temp.shouldDie) {
							shouldBeKilled = true;
						}
						if (check_temp.otherShouldDie) {
							if (tanks[i]->getID() != tanks[j]->getID()) {
								killOtherTank = true;
							}
						}
					}
				}

				if (!overridedTankCollision) {
					CollisionHandler::pushMovableAwayFromMovable(tanks[i], tanks[j]);
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
	Diagnostics::endTiming();

	Diagnostics::startTiming();
	Diagnostics::addName("tank-edge");
	//tank to edge collision:
	for (int i = 0; i < tanks.size(); i++) {
		bool shouldBeKilled = false;
		
		bool modifiedEdgeCollision = false;
		bool overridedEdgeCollision = false;
		bool noMoreEdgeCollisionSpecials = false;

		if (tanks[i]->isPartiallyOutOfBounds()) {
			for (int k = 0; k < tanks[i]->tankPowers.size(); k++) {
				if (tanks[i]->tankPowers[k]->modifiesCollisionWithEdge) {
					if (tanks[i]->tankPowers[k]->modifiedEdgeCollisionCanOnlyWorkIndividually && modifiedEdgeCollision) {
						continue;
					}
					if (noMoreEdgeCollisionSpecials) {
						continue;
					}

					modifiedEdgeCollision = true;
					if (tanks[i]->tankPowers[k]->overridesEdgeCollision) {
						overridedEdgeCollision = true;
					}
					if (!tanks[i]->tankPowers[k]->modifiedEdgeCollisionCanWorkWithOthers) {
						noMoreEdgeCollisionSpecials = true;
					}

					PowerInteractionBoolHolder check_temp = tanks[i]->tankPowers[k]->modifiedEdgeCollision(tanks[i]);
					if (check_temp.shouldDie) {
						shouldBeKilled = true;
					}
				}
			}

			if (!overridedEdgeCollision) {
				tanks[i]->edgeConstrain();
			}
		}

		if (shouldBeKilled) {
			tank_dead = 1;
		}
	}
	Diagnostics::endTiming();

	Diagnostics::startTiming();
	Diagnostics::addName("bullet-edge");
	//bullet to edge collision:
	for (int i = bullets.size() - 1; i >= 0; i--) {
		bool modifiedEdgeCollision = false;
		bool overridedEdgeCollision = false;
		bool noMoreEdgeCollisionSpecials = false;
		bool shouldBeKilled = false;

		if (bullets[i]->isPartiallyOutOfBounds()) {
			for (int k = 0; k < bullets[i]->bulletPowers.size(); k++) {
				if (bullets[i]->bulletPowers[k]->modifiesCollisionWithEdge) {
					if (bullets[i]->bulletPowers[k]->modifiedEdgeCollisionCanOnlyWorkIndividually && modifiedEdgeCollision) {
						continue;
					}
					if (noMoreEdgeCollisionSpecials) {
						continue;
					}

					modifiedEdgeCollision = true;
					if (bullets[i]->bulletPowers[k]->overridesEdgeCollision) {
						overridedEdgeCollision = true;
					}
					if (!bullets[i]->bulletPowers[k]->modifiedEdgeCollisionCanWorkWithOthers) {
						noMoreEdgeCollisionSpecials = true;
					}

					PowerInteractionBoolHolder check_temp = bullets[i]->bulletPowers[k]->modifiedEdgeCollision(bullets[i]);
					if (check_temp.shouldDie) {
						shouldBeKilled = true;
					}
				}
			}

			if (overridedEdgeCollision) {
				continue;
			}
			if (bullets[i]->isFullyOutOfBounds()) {
				shouldBeKilled = true;
			}
		}

		if (shouldBeKilled) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
			continue;
		}
	}
	Diagnostics::endTiming();

	Diagnostics::startTiming();
	Diagnostics::addName("bullet-wall");
	//bullet to wall collision:
	for (int i = bullets.size() - 1; i >= 0; i--) {
		bool shouldBeKilled = false;

		for (int j = walls.size() - 1; j >= 0; j--) {
			bool modifiedWallCollision = false;
			bool overridedWallCollision = false;
			bool noMoreWallCollisionSpecials = false;
			bool killWall = false;

			if (CollisionHandler::partiallyCollided(bullets[i], walls[j])) {
				for (int k = 0; k < bullets[i]->bulletPowers.size(); k++) {
					if (bullets[i]->bulletPowers[k]->modifiesCollisionWithWall) {
						if (bullets[i]->bulletPowers[k]->modifiedCollisionWithWallCanOnlyWorkIndividually && modifiedWallCollision) {
							continue;
						}
						if (noMoreWallCollisionSpecials) {
							continue;
						}

						modifiedWallCollision = true;
						if (bullets[i]->bulletPowers[k]->overridesCollisionWithWall) {
							overridedWallCollision = true;
						}
						if (!bullets[i]->bulletPowers[k]->modifiedCollisionWithWallCanWorkWithOthers) {
							noMoreWallCollisionSpecials = true;
						}

						PowerInteractionBoolHolder check_temp = bullets[i]->bulletPowers[k]->modifiedCollisionWithWall(bullets[i], walls[j]);
						if (check_temp.shouldDie) {
							shouldBeKilled = true;
						}
						if (check_temp.otherShouldDie) {
							killWall = true;
						}
					}
				}

				if (!overridedWallCollision) {
					if (CollisionHandler::partiallyCollided(bullets[i], walls[j])) {
						shouldBeKilled = true;
					}
				}
			}

			if (killWall) {
				delete walls[j];
				walls.erase(walls.begin() + j);
			}
		}

		if (shouldBeKilled) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
	}
	Diagnostics::endTiming();
	//bullet to wall is a big timesink, but it can only really be sped up by multithreading

	Diagnostics::startTiming();
	Diagnostics::addName("bullet-bullet");
	//bullet collision:
	//TODO: modernize (add default vs custom collision stuff)
	for (int i = bullets.size() - 1; i >= 0; i--) {
		for (int j = bullets.size() - 1; j >= 0; j--) { //could start at i-1? //fix: find out
			if (bullets[i]->getID() == bullets[j]->getID()) {
				continue;
			}
			if (CollisionHandler::partiallyCollided(bullets[i], bullets[j])) {
				char result = BulletPriorityHandler::determinePriority(bullets[i], bullets[j]);
				if (result <= -2) {
					bool firstDies = rand()%2;
					if (firstDies) {
						delete bullets[i];
						bullets.erase(bullets.begin() + i);
						break;
					} else {
						delete bullets[j];
						bullets.erase(bullets.begin() + j);
						continue; //fix: is this supposed to be break?
					}
				} else if (result == -1) { //both die
					delete bullets[i];
					delete bullets[j];
					if (i > j) {
						bullets.erase(bullets.begin() + i);
						bullets.erase(bullets.begin() + j);
						i--;
					} else {
						bullets.erase(bullets.begin() + j);
						bullets.erase(bullets.begin() + i);
					}
					break;
				} else if (result >= 2) { //it's a draw, so neither dies
					//continue;
				} else {
					if (result == 0) {
						delete bullets[i];
						bullets.erase(bullets.begin() + i);
						i--;
						break;
					} else {
						delete bullets[j];
						bullets.erase(bullets.begin() + j);
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
	Diagnostics::endTiming();
	//bullet to bullet collision is the biggest timesink (obviously)
	//unfortunately it can only be O(n^2), and multithreading doesn't seem like it would work

	Diagnostics::startTiming();
	Diagnostics::addName("bullet-tank");
	//bullet to tank collision:
	//TODO: modernize (add default vs custom collision stuff)
	for (int i = 0; i < tanks.size(); i++) {
		for (int j = 0; j < bullets.size(); j++) {
			if (bullets[j]->getID() == tanks[i]->getID()) {
				continue;
			}
			if (CollisionHandler::partiallyCollided(tanks[i], bullets[j])) {
				char result = BulletPriorityHandler::determinePriority(bullets[j], tanks[i]);
				if (result <= -2) {
					bool firstDies = rand()%2;
					if (firstDies) {
						tank_dead = 1;
						continue;
					} else {
						delete bullets[j];
						bullets.erase(bullets.begin() + j);
						j--;
						continue; //fix: is this supposed to be break?
					}
				} else if (result == -1) { //both die
					tank_dead = 1;

					/*
					delete bullets[j];
					bullets.erase(bullets.begin() + j);
					j--;
					*/

					continue;
				} else if (result >= 2) { //it's a draw, so neither dies (probably not going to happen)
					//continue;
				} else {
					if (result == 0) {
						delete bullets[j];
						bullets.erase(bullets.begin() + j);
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
	Diagnostics::endTiming();

	//edge constrain tanks again in case bullet decides to move tank
	//don't edge constrain if said tank is dead //fix: implement
	/*
	for (int i = 0; i < tanks.size(); i++) {
		//if (tanks[i]->isOutOfBounds())) {
			tanks[i]->edgeConstrain();
		//}
	}
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



int main(int argc, char** argv) {

	srand(time(NULL));

	//TODO: move some of this stuff to an initialization file so testing can be done
	normalKeyStates.insert({ 'w', false });
	normalKeyStates.insert({ 'a', false });
	normalKeyStates.insert({ 's', false });
	normalKeyStates.insert({ 'd', false });
	specialKeyStates.insert({ GLUT_KEY_UP, false });
	specialKeyStates.insert({ GLUT_KEY_LEFT, false });
	specialKeyStates.insert({ GLUT_KEY_RIGHT, false });
	specialKeyStates.insert({ GLUT_KEY_DOWN, false });

	levelLookup.insert({ "random", new RandomLevel() });
	levelLookup.insert({ "empty", new EmptyLevel() });
	levelLookup.insert({ "corridor", new CorridorLevel() });

	powerLookup.insert({ "speed",  SpeedPower::factory });
	powerLookup.insert({ "wallhack",  WallhackPower::factory });
	powerLookup.insert({ "bounce",  BouncePower::factory });
	powerLookup.insert({ "multishot", MultishotPower::factory });
	powerLookup.insert({ "triple", TriplePower::factory });
	powerLookup.insert({ "homing", HomingPower::factory });
	powerLookup.insert({ "invincible", InvincibleNamedPower::factory });
	powerLookup.insert({ "big", BigNamedPower::factory });
	powerLookup.insert({ "megadeath", MegaDeathPower::factory });
	powerLookup.insert({ "grenade", GrenadePower::factory });

	tanks.push_back(tank1);
	tanks.push_back(tank2);

	bullets.reserve(800);

	/*
	for (int i = 0; i < 4; i++) {
		walls.push_back(new Wall(320 - 240*(((3-i)/2) * 2 - 1) - 32*((((3-i)/2) + 1) % 2), i%2 * (320-128), 32, 128, ColorValueHolder(255,0,255)));
	}
	*/

	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	//thanks to https://community.khronos.org/t/wglmakecurrent-issues/62656/3 for solving why a draw call would take ~15ms for no reason

	// Setup window position, size, and title
	glutInitWindowPosition(60, 60);
	glutInitWindowSize(width, height);
	glutCreateWindow("Tanks Test");

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

	//TODO: proper solution
	tanks[0]->determineShootingAngles();
	tanks[1]->determineShootingAngles();
	levelLookup["random"]->initialize();


	//make the classes load their vertices and indices onto VRAM to avoid CPU<->GPU syncs
	Renderer::Initialize();
	Bullet::initializeGPU();
	BackgroundRect::initializeGPU();
	PowerSquare::initializeGPU();
	Wall::initializeGPU();
	Tank::initializeGPU();


	// Set callback for drawing the scene
	glutDisplayFunc(appDrawScene);

	// Set callback for resizing the window
	glutReshapeFunc(appReshapeFunc);

	//mouse clicking
	glutMouseFunc(mouse_func);

	// Set callback to handle mouse dragging
	glutMotionFunc(appMotionFunc);

	// Set callback to handle keyboard events
	glutKeyboardFunc(keyboard_down);

	//callback for keyboard up events
	glutKeyboardUpFunc(keyboard_up);

	//special keyboard down
	glutSpecialFunc(special_keyboard_down);

	//special keyboard up
	glutSpecialUpFunc(special_keyboard_up);

	// Set callback for the idle function
	//glutIdleFunc(draw);

	//framelimiter
	glutTimerFunc(1000/physicsRate, tick, physicsRate);

	// Start the main loop
	glutMainLoop();

}

/*
 * estimated total completion:
 * 100% required GPU drawing stuff!
 * 20% theoretical GPU stuff (may not attempt)
 * * gotta learn how to do batching
 * * add a gradient shader
 * * make things more efficient (way easier said than done, I suppose)
 * * * where do I even start (besides batching)?
 * * * can have rect and circle store their stuff, then have every drawing thing just scale and rotate as needed
 * 80% theoretical foundation: no hazards
 * 60% actual foundation: not every "modification function" actually does something in the main
 * 20% game code:
 * * first off, don't know what will be final beyond the ideas located in power.h and elsewhere
 * * second, it's a complete estimate (obviously) and this is a restatement of the first
 * * third, 100% probably won't be "finished" on this scale (restatement of the second?)
 * * fourth, percentage is horribly imprecise because, like most people, I think about completion percentages on personal projects in 5% increments (restatement of third)
 * * fifth, here's what's next:
 * * * invincibility series of powerups (overhaul priority handling)
 * * * even later: newer levels
 */
