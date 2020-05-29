#pragma once
#include "horizontallightning.h"
#include "gamemanager.h"
#include "renderer.h"
#include "backgroundrect.h"
#include "colormixer.h"
#include "mylib.h"
#include "constants.h"
#include <math.h>
#include <stdexcept>
#include <algorithm>
#include "point.h"
#include "collisionhandler.h"
#include "wallmanager.h"
#include <iostream>

VertexArray* HorizontalLightning::background_va;
VertexBuffer* HorizontalLightning::background_vb;
IndexBuffer* HorizontalLightning::background_ib;
bool HorizontalLightning::initialized_GPU = false;

HorizontalLightning::HorizontalLightning(double xpos, double ypos, double width, double height) {
	x = xpos;
	y = ypos;
	w = width;
	h = height;
	gameID = GameManager::getNextID();
	teamID = HAZARD_TEAM;

	tickCycle = 100; //100 is JS default (because of power speed)
	double temp[2] = { 2, 2 };
	std::copy(temp, temp+2, stateMultiplier);
	currentlyActive = false;
	//flexible = false;

	leftSide  = new Point(x,     y + h/2);
	rightSide = new Point(x + w, y + h/2);
	
	lengthOfBolt = 4; //TODO: figure out logic for constraining this to make it look pretty
	bolts.reserve(maxBolts);
	int boltPoints = getDefaultNumBoltPoints(w);
	for (int i = 0; i < maxBolts; i++) {
		pushBolt(new LightningBolt(0, h/2, w, h/2, boltPoints), true);
	}

	canAcceptPowers = false;

	modifiesTankCollision = true;
	hasSpecialEffectTankCollision = true;
	modifiesBulletCollision = true;
	hasSpecialEffectBulletCollision = true;

	local_initializeGPU();
	initializeGPU();
}

int HorizontalLightning::getDefaultNumBoltPoints(double horzDist) {
	int boltPoints = ceil(horzDist / lengthOfBolt); //not floor because the last point is the edge of the lightning area
	return (boltPoints < 2 ? 2 : boltPoints);
}

HorizontalLightning::~HorizontalLightning() {
	clearBolts();
	delete leftSide, rightSide;

	local_uninitializeGPU();
	//uninitializeGPU();
}

bool HorizontalLightning::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	float positions[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	background_vb = new VertexBuffer(positions, 4*2 * sizeof(float), GL_DYNAMIC_DRAW);
	VertexBufferLayout layout(2);
	background_va = new VertexArray(*background_vb, layout);

	background_ib = new IndexBuffer(indices, 6);

	initialized_GPU = true;
	return true;
}

void HorizontalLightning::local_initializeGPU() {
	float* positions = new float[bolts[0]->length*2];
	for (int i = 0; i < bolts[0]->length; i++) {
		positions[i*2]   = bolts[0]->positions[i*2];
		positions[i*2+1] = bolts[0]->positions[i*2+1];
	}
	bolt_vb_length = bolts[0]->length;
	
	bolt_vb = new VertexBuffer(positions, bolts[0]->length*2 * sizeof(float), GL_STREAM_DRAW);
	VertexBufferLayout layout(2);
	bolt_va = new VertexArray(*bolt_vb, layout);

	delete[] positions;
}

void HorizontalLightning::local_reinitializeGPU(int length) { //does not seed the VertexBuffer with values
	delete bolt_va;
	delete bolt_vb;

	float* positions = new float[length*2];
	bolt_vb_length = length;
	
	bolt_vb = new VertexBuffer(positions, length*2 * sizeof(float), GL_STREAM_DRAW);
	VertexBufferLayout layout(2);
	bolt_va = new VertexArray(*bolt_vb, layout);

	delete[] positions;
}

bool HorizontalLightning::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete background_va;
	delete background_vb;
	delete background_ib;

	initialized_GPU = false;
	return true;
}

void HorizontalLightning::local_uninitializeGPU() {
	delete bolt_va;
	delete bolt_vb;
}

void HorizontalLightning::streamBoltVertices(unsigned int boltNum) {
	bolt_vb->modifyData(&bolts[boltNum]->positions[0], bolts[boltNum]->length*2 * sizeof(float));
}

RectHazard* HorizontalLightning::factory(int argc, std::string* argv) {
	if (argc >= 4) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double w = std::stod(argv[2]);
		double h = std::stod(argv[3]);
		return new HorizontalLightning(x, y, w, h);
	}
	return new HorizontalLightning(0, 0, 0, 0);
}

void HorizontalLightning::tick() {
	if (!validLocation()) {
		tickCount = 0;
		currentlyActive = false;
		targetedObjects.clear();
		clearBolts();
		return;
	}

	tickCount++;
	if (tickCount >= tickCycle * stateMultiplier[currentlyActive]) {
		if (tickCycle * stateMultiplier[currentlyActive] <= 0) {
			tickCount = 0;
			currentlyActive = true;
		} else {
			tickCount -= tickCycle * stateMultiplier[currentlyActive];
			currentlyActive = !currentlyActive;
		}
	}
	
	if (currentlyActive) {
		if (boltTick >= boltCycle) {
			//hazard tick comes before collision, therefore there will be more bolt refreshes after this if a bullet/tank collides
			targetedObjects.clear();
			boltsNeeded = false;
			clearBolts();
			int boltPoints = getDefaultNumBoltPoints(w);
			for (int i = 0; i < maxBolts; i++) {
				pushBolt(new LightningBolt(0, h/2, w, h/2, boltPoints), true);
			}
			boltTick = 0;
		}
		boltTick++;
	} else {
		boltTick = boltCycle; //start at boltCycle to force a refresh as soon as possible
	}
}

void HorizontalLightning::specialEffectCircleCollision(Circle* c) {
	/* Lightning zone/region layout:
	 *
	 *       6aL      |  3aL  |               2a             |  3aR  |       6aR
	 *                |       |                              |       |
	 *----------------+-------+------------------------------+-------+-----------------      <- y+h
	 *                |       |                              |       |
	 *       5L       0  4L   |               1              |  4R   0       5R
	 *                |       |                              |       |
	 *----------------+-------+------------------------------+-------+-----------------      <- y
	 *                |       |                              |       |
	 *       6bL      |  3bL  |               2b             |  3bR  |       6bR
	 *
	 *
	 *                ^       ^                              ^       ^
	 *                x     x + c->r                     x+w - c->r  x+w
	 *
	 * 0 = leftStart/rightStart
	 * zone/region 7: circle radius c->r around 0
	 * determined by circle center
	 */

	//TODO: confirm everything is good
	double intersectionXL, intersectionXR, intersectionYL, intersectionYR;
	int boltPointsL = -1, boltPointsR = -1;
	//determine region the circle is in
	if (c->x-c->r >= x && c->x+c->r <= x+w) {
		if (c->y >= y && c->y <= y+h) {
			intersectionXL = c->x - c->r;
			intersectionXR = c->x + c->r;
			intersectionYL = c->y;
			intersectionYR = c->y;
			//std::cout << "case 1" << std::endl;
		} else {
			if (c->y > y+h) {
				intersectionYL = y + h;
				intersectionYR = y + h;
				//std::cout << "case 2a" << std::endl;
			} else { //c->y < y
				intersectionYL = y;
				intersectionYR = y;
				//std::cout << "case 2b" << std::endl;
			}
			intersectionXL = c->x - sqrt(pow(c->r, 2) - pow(intersectionYL - c->y, 2));
			//intersectionXL = (intersectionXL < x ? x : intersectionXL);
			intersectionXR = c->x + sqrt(pow(c->r, 2) - pow(intersectionYR - c->y, 2));
			//intersectionXR = (intersectionXR > x+w ? x+w : intersectionXR);
		}
	} else {
		//I think case 4 and 5 are the same
		if (c->x-c->r < x) {
			//left

			if (CollisionHandler::fullyCollided(leftSide, c)) {
				intersectionXL = c->x;
				intersectionYL = c->y;
				boltPointsL = 2;
				if (c->y > y+h) {
					intersectionXR = c->x + sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
					intersectionYR = y + h;
				} else if(c->y < y) {
					intersectionXR = c->x + sqrt(pow(c->r, 2) - pow(c->y - y, 2));
					intersectionYR = y;
				} else {
					intersectionXR = c->x + c->r;
					intersectionYR = c->y;
				}
				//std::cout << "case 7L_L" << std::endl;
			} else {
				//check whether the circle is really far to the left, or just barely out of range of leftSide
				//difference between 6aL and 3aL: intersectionXL
				if (c->x <= x) {
					if (c->y > y+h) {
						intersectionXL = x;
						intersectionYL = c->y - sqrt(pow(c->r, 2) - pow(c->x - x, 2));
						intersectionXR = c->x + sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
						intersectionYR = y + h;
						//std::cout << "case 6aL ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else if(c->y < y) {
						intersectionXL = x;
						intersectionYL = c->y + sqrt(pow(c->r, 2) - pow(c->x - x, 2));
						intersectionXR = c->x + sqrt(pow(c->r, 2) - pow(c->y - y, 2));
						intersectionYR = y;
						//std::cout << "case 6bL ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else {
						intersectionXL = x;
						if (c->y > y + h/2) {
							intersectionYL = c->y - sqrt(pow(c->r, 2) - pow(c->x - x, 2));
						} else {
							intersectionYL = c->y + sqrt(pow(c->r, 2) - pow(c->x - x, 2));
						}
						intersectionXR = c->x + c->r;
						intersectionYR = c->y;
						//std::cout << "case 5L ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					}
				} else {
					if (c->y > y+h) {
						intersectionXL = c->x - sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
						intersectionXL = (intersectionXL < x ? x : intersectionXL);
						intersectionYL = c->y - sqrt(pow(c->r, 2) - pow(c->x - x, 2));
						intersectionYL = (intersectionYL > y+h ? y+h : intersectionYL);
						intersectionXR = c->x + sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
						intersectionYR = y + h;
						//std::cout << "case 3aL ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else if(c->y < y) {
						intersectionXL = c->x - sqrt(pow(c->r, 2) - pow(c->y - y, 2));
						intersectionXL = (intersectionXL < x ? x : intersectionXL);
						intersectionYL = c->y + sqrt(pow(c->r, 2) - pow(c->x - x, 2));
						intersectionYL = (intersectionYL < y ? y : intersectionYL);
						intersectionXR = c->x + sqrt(pow(c->r, 2) - pow(c->y - y, 2));
						intersectionYR = y;
						//std::cout << "case 3bL ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else {
						intersectionXL = x;
						if (c->y > y + h/2) {
							intersectionYL = c->y - sqrt(pow(c->r, 2) - pow(c->x - x, 2));
						} else {
							intersectionYL = c->y + sqrt(pow(c->r, 2) - pow(c->x - x, 2));
						}
						intersectionXR = c->x + c->r;
						intersectionYR = c->y;
						//std::cout << "case 4L ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					}
				}
			}

			if (CollisionHandler::fullyCollided(rightSide, c)) { //this should be very rare (only happens for small lightnings)
				intersectionXR = c->x;
				intersectionYR = c->y;
				boltPointsR = 2;
				//std::cout << "case 7R_L" << std::endl;
			}
		} else { //c->x+c->r > x+w
			//right

			if (CollisionHandler::fullyCollided(rightSide, c)) {
				intersectionXR = c->x;
				intersectionYR = c->y;
				boltPointsR = 2;
				if (c->y > y+h) {
					intersectionXL = c->x - sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
					intersectionYL = y + h;
				} else if(c->y < y) {
					intersectionXL = c->x - sqrt(pow(c->r, 2) - pow(c->y - y, 2));
					intersectionYL = y;
				} else {
					intersectionXL = c->x - c->r;
					intersectionYL = c->y;
				}
				//std::cout << "case 7R_R" << std::endl;
			} else {
				if (c->x >= x+w) {
					if (c->y > y+h) {
						intersectionXL = c->x - sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
						intersectionYL = y + h;
						intersectionXR = x + w;
						intersectionYR = c->y - sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
						//std::cout << "case 6aR ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else if(c->y < y) {
						intersectionXL = c->x - sqrt(pow(c->r, 2) - pow(c->y - y, 2));
						intersectionYL = y;
						intersectionXR = x + w;
						intersectionYR = c->y + sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
						//std::cout << "case 6bR ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else {
						intersectionXL = c->x - c->r;
						intersectionYL = c->y;
						intersectionXR = x + w;
						if (c->y > y + h/2) {
							intersectionYR = c->y - sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
						} else {
							intersectionYR = c->y + sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
						}
						//std::cout << "case 5R ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					}
				} else {
					if (c->y > y+h) {
						intersectionXL = c->x - sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
						intersectionYL = y + h;
						intersectionXR = c->x + sqrt(pow(c->r, 2) - pow(c->y - (y+h), 2));
						intersectionXR = (intersectionXR > x+w ? x+w : intersectionXR);
						intersectionYR = c->y - sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
						intersectionYR = (intersectionYR > y+h ? y+h : intersectionYR);
						//std::cout << "case 3aR ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else if(c->y < y) {
						intersectionXL = c->x - sqrt(pow(c->r, 2) - pow(c->y - y, 2));
						intersectionYL = y;
						intersectionXR = c->x + sqrt(pow(c->r, 2) - pow(c->y - y, 2));
						intersectionXR = (intersectionXR > x+w ? x+w : intersectionXR);
						intersectionYR = c->y + sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
						intersectionYR = (intersectionYR < y ? y : intersectionYR);
						//std::cout << "case 3bR ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else {
						intersectionXL = c->x - c->r;
						intersectionYL = c->y;
						intersectionXR = x + w;
						if (c->y > y + h/2) {
							intersectionYR = c->y - sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
						} else {
							intersectionYR = c->y + sqrt(pow(c->r, 2) - pow(c->x - (x+w), 2));
						}
						//std::cout << "case 4R ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					}
				}
			}

			if (CollisionHandler::fullyCollided(leftSide, c)) { //this should be very rare (only happens for small lightnings)
				intersectionXL = c->x;
				intersectionYL = c->y;
				boltPointsL = 2;
				//std::cout << "case 7L_R" << std::endl;
			}
		}
	}
	double distL = sqrt(pow(intersectionXL - x, 2) + pow(intersectionYL - (y + h/2), 2));
	double distR = sqrt(pow((x + w) - intersectionXR, 2) + pow((y + h/2) - intersectionYR, 2));
	boltPointsL = (boltPointsL < 2 ? getDefaultNumBoltPoints(distL) : boltPointsL);
	boltPointsR = (boltPointsR < 2 ? getDefaultNumBoltPoints(distR) : boltPointsR);
	pushBolt(new LightningBolt(0, h/2, intersectionXL-x, intersectionYL-y, boltPointsL), false);
	pushBolt(new LightningBolt(intersectionXL-x, intersectionYL-y, intersectionXR-x, intersectionYR-y, 2), false);
	pushBolt(new LightningBolt(intersectionXR-x, intersectionYR-y, w, h/2, boltPointsR), false);

	//compared to JS Tanks, this intersection logic is much more complex
	//that's okay because wallhacking into a lightning will look less weird

	//TODO: is this complete?
	//probably, but still needs a refactor
}

void HorizontalLightning::specialEffectTankCollision(Tank* t) {
	//if bolts are being randomized, clear them, and mark that they've been cleared
	if (!boltsNeeded) {
		clearBolts();
	}
	boltsNeeded = true;

	if (std::find(targetedObjects.begin(), targetedObjects.end(), t->getGameID()) == targetedObjects.end()) {
		targetedObjects.push_back(t->getGameID());
	} else {
		return;
	}

	specialEffectCircleCollision(t);
}

void HorizontalLightning::specialEffectBulletCollision(Bullet* b) {
	if (!boltsNeeded) {
		clearBolts();
	}
	boltsNeeded = true;

	if (std::find(targetedObjects.begin(), targetedObjects.end(), b->getGameID()) == targetedObjects.end()) {
		targetedObjects.push_back(b->getGameID());
	} else {
		return;
	}

	specialEffectCircleCollision(b);
}

void HorizontalLightning::pushBolt(LightningBolt* l, bool simpleRefresh) {
	if (l->length > bolt_vb_length) {
		local_reinitializeGPU(l->length);
	}
	bolts.push_back(l);
	if (simpleRefresh) {
		simpleRefreshBolt(bolts.size() - 1);
	} else {
		refreshBolt(bolts.size() - 1);
	}
}

bool HorizontalLightning::validLocation() {
	bool wallOnLeft = false, wallOnRight = false, wallInMiddle = false;
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		//I'm not sure numerical comparisons are "costly" to the point where short circuiting is needed
		if (!(wallOnLeft && wallOnRight) && (wa->y <= y) && (wa->y + wa->h >= y + h)) {
			if (!wallOnLeft && (x == wa->x + wa->w)) {
				wallOnLeft = true;
			} else if (!wallOnRight && (x + w == wa->x)) {
				wallOnRight = true;
			}
		}
		if (CollisionHandler::partiallyCollidedIgnoreEdge(wa, this)) {
			wallInMiddle = true;
			break;
		}
	}
	if (!wallOnLeft && (x == 0)) {
		wallOnLeft = true;
	}
	if (!wallOnRight && (x + w == GAME_WIDTH)) {
		wallOnRight = true;
	}
	return (wallOnLeft && wallOnRight && !wallInMiddle);
}

bool HorizontalLightning::reasonableLocation() {
	bool wallOnTop = false, wallOnBottom = false;
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		if ((wa->x <= x) && (wa->x + wa->w >= x + w)) {
			if (!wallOnBottom && (y == wa->y + wa->h)) {
				wallOnBottom = true;
			} else if (!wallOnTop && (y + h == wa->y)) {
				wallOnTop = true;
			}
		}
		if (wallOnTop && wallOnBottom) {
			break;
		}
	}
	if (!wallOnBottom && (y <= 0)) { //should this be ==? //also, should this function return false if the lightning is out of bounds?
		wallOnBottom = true;
	}
	if (!wallOnTop && (y + h >= GAME_HEIGHT)) {
		wallOnTop = true;
	}
	return (!(wallOnTop && wallOnBottom) && validLocation());
}

void HorizontalLightning::simpleRefreshBolt(int num) {
	double maxVariance = h/4;
	/* lightning bolts are allowed to be in an area that looks like this:
	 * 
	 * +   --------------   +        <- 1.0
	 * |  /              \  |
	 * | /                \ |
	 * |<       HERE       >|
	 * | \                / |
	 * |  \              /  |
	 * +   --------------   +        <- 0.0
	 * 
	 * ^                    ^
	 * 0.0                  1.0
	 * 
	 * the region is 1/4 triangle, 1/2 rectangle, then 1/4 triangle
	 */

	//old method:
	/*
	//old method note: x and y coordinates in range [0,1], maxVariance = 1.0/4.0
	for (int j = 1; j < bolts[num]->length-1; j++) {
		//bolts[num]->positions[j*2]   = float(j)/(bolts[num]->length - 1);
		//bolts[num]->positions[j*2+1] = randFunc2();
		double testPoint; //y-position of the new point
		if(j < bolts[num]->length / 4){ //first quarter
			do{
				testPoint = bolts[num]->positions[j*2 - 1] + (randFunc2()*2-1) * maxVariance;
			}while(testPoint <= -2 * (double(j) / bolts[num]->length) + .5 || //"below" the triangle (just in slope-intercept form, nothing special)
			       testPoint >=  2 * (double(j) / bolts[num]->length) + .5);  //"above" the triangle
		}else if(j < bolts[num]->length * 3.0/4.0){ //middle half
			do{
				testPoint = bolts[num]->positions[j*2 - 1] + (randFunc2()*2-1) * maxVariance;
			}while(testPoint >= 1 || testPoint <= 0);
		}else{ //last quarter
			do{
				testPoint = bolts[num]->positions[j*2 - 1] + (randFunc2()*2-1) * maxVariance;
			}while(testPoint <=  2 * (double(j) / bolts[num]->length - 3.0/4.0) + 0 ||
			       testPoint >= -2 * (double(j) / bolts[num]->length - 3.0/4.0) + 1);
		}
		bolts[num]->positions[j*2+1] = testPoint;
	}
	*/

	double deltaX = (bolts[num]->positions[bolts[num]->length*2-2] - bolts[num]->positions[0]) / (bolts[num]->length - 1);
	for (int j = 1; j < bolts[num]->length-1; j++) {
		double yRangeLower = bolts[num]->positions[j*2 - 1] - maxVariance;
		double yRangeUpper = bolts[num]->positions[j*2 - 1] + maxVariance;
		double yMin, yMax;
		if(j < bolts[num]->length / 4){ //first quarter
			yMin = -2*h/w * (deltaX * j) + h/2;
			yMax =  2*h/w * (deltaX * j) + h/2;
		}else if(j < bolts[num]->length * 3.0/4.0){ //middle half
			yMin = 0;
			yMax = h;
		}else{ //last quarter
			yMin =  2*h/w * (deltaX * (j - bolts[num]->length*3.0/4.0)) + 0;
			yMax = -2*h/w * (deltaX * (j - bolts[num]->length*3.0/4.0)) + h;
		}
		yRangeLower = (yRangeLower < yMin ? yMin : yRangeLower);
		yRangeUpper = (yRangeUpper > yMax ? yMax : yRangeUpper);
		bolts[num]->positions[j*2+1] = yRangeLower + (yRangeUpper - yRangeLower) * randFunc2();
	}
}

void HorizontalLightning::refreshBolts() {
	for (int i = 0; i < bolts.size(); i++) {
		refreshBolt(i);
	}
}

void HorizontalLightning::refreshBolt(int num) {
	if (bolts[num]->length <= 2) {
		return;
	}

	float deltaX = bolts[num]->positions[bolts[num]->length*2-2] - bolts[num]->positions[0];
	float deltaY = bolts[num]->positions[bolts[num]->length*2-1] - bolts[num]->positions[1];
	double dist = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
	double rotationAngle = atan2(deltaY, deltaX);
	double angleSin = sin(rotationAngle);
	double angleCos = cos(rotationAngle);
	double newH = dist * h/w;
	double maxVariance = 1.0/4.0 * dist * h/w;

	float polygonX[6] = {
		bolts[num]->positions[0],
		bolts[num]->positions[0] + deltaX * 1.0/4.0 - angleSin * newH * .5,
		bolts[num]->positions[0] + deltaX * 3.0/4.0 - angleSin * newH * .5,
		bolts[num]->positions[0] + deltaX,
		bolts[num]->positions[0] + deltaX * 3.0/4.0 + angleSin * newH * .5,
		bolts[num]->positions[0] + deltaX * 1.0/4.0 + angleSin * newH * .5
	};
	float polygonY[6] = {
		bolts[num]->positions[1],
		bolts[num]->positions[1] + deltaY * 1.0/4.0 + angleCos * newH * .5,
		bolts[num]->positions[1] + deltaY * 3.0/4.0 + angleCos * newH * .5,
		bolts[num]->positions[1] + deltaY,
		bolts[num]->positions[1] + deltaY * 3.0/4.0 - angleCos * newH * .5,
		bolts[num]->positions[1] + deltaY * 1.0/4.0 - angleCos * newH * .5
	};

	//std::cout << "deltaX: " << deltaX << std::endl;
	//std::cout << "deltaY: " << deltaY << std::endl;
	//std::cout << "deltaY adj: " << (deltaY * h/w) << std::endl;
	//std::cout << "dist: " << dist << std::endl;
	//std::cout << "angle: " << (rotationAngle * 180/3.1415926535897) << std::endl;
	//std::cout << "cos(angle): " << angleCos << std::endl;
	//std::cout << "sin(angle): " << angleSin << std::endl;
	for (int i = 0; i < 6; i++) {
		//std::cout << i << ": " << polygonX[i] << " " << polygonY[i] << std::endl;
	}

	for (int j = 1; j < bolts[num]->length-1; j++) {
		double randTemp;
		float testY, testX;
		do {
			randTemp = (randFunc2()*2-1)*maxVariance;
			testY = bolts[num]->positions[j*2 - 1] + (deltaY/(bolts[num]->length-1)) + randTemp * angleCos;
			testX = bolts[num]->positions[j*2 - 2] + (deltaX/(bolts[num]->length-1)) - randTemp * angleSin;
			//std::cout << testX << " " << testY << std::endl;
		} while (testY < 0 || testY > h || testX < 0 || testX > w || !pointInPolygon(6, polygonX, polygonY, testX, testY));
		bolts[num]->positions[j*2]   = testX;
		bolts[num]->positions[j*2+1] = testY;
	}
}

void HorizontalLightning::clearBolts() {
	for (int i = 0; i < bolts.size(); i++) {
		delete bolts[i];
	}
	bolts.clear();
}

ColorValueHolder HorizontalLightning::getBackgroundColor() {
	if (currentlyActive) {
		return ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(.75f, .75f, .75f), .25);
	}
	return ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(.75f, .75f, .75f), .25*constrain_d(tickCount/(tickCycle*stateMultiplier[currentlyActive]), 0, 1));
}

ColorValueHolder HorizontalLightning::getBoltColor() {
	return ColorValueHolder(0xBB/255.0, 1.0f, 1.0f);
}

void HorizontalLightning::draw() {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(w, h, 0, x, y);
	
	//background:
	//TODO: make drawUnder() a thing
	ColorValueHolder color = getBackgroundColor();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*background_va, *background_ib, *shader);

	//bolts:
	if (!currentlyActive) {
		return;
	}

	glLineWidth(2.0f);
	color = getBoltColor();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	MVPM = Renderer::GenerateMatrix(1, 1, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	for (int i = 0; i < bolts.size(); i++) {
		//I think the VertexBuffer resizing should happen here, but there would probably be less strain if it happens only when a bullet/tank collides
		/*
		if (bolts[i]->length > bolt_vb_length) {
			local_reinitializeGPU(bolts[i]->length);
		}
		*/
		streamBoltVertices(i);
		Renderer::Draw(*bolt_va, *shader, GL_LINE_STRIP, 0, bolts[i]->length);
	}
}

void HorizontalLightning::drawCPU() {
	//background:

	//bolts:

}
