#pragma once
#include "horizontallightning.h"
#include "renderer.h"
#include "backgroundrect.h"
#include "colormixer.h"
#include "mylib.h"
#include <math.h>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "point.h"
#include "collisionhandler.h"

VertexArray* HorizontalLightning::background_va;
VertexBuffer* HorizontalLightning::background_vb;
IndexBuffer* HorizontalLightning::background_ib;
bool HorizontalLightning::initialized_GPU = false;

HorizontalLightning::HorizontalLightning(double xpos, double ypos, double width, double height) {
	x = xpos;
	y = ypos;
	w = width;
	h = height;

	tickCycle = 100; //100 is JS default (because of power speed)
	stateMultiplier[0] = 2, stateMultiplier[1] = 2; //don't forget about std::copy
	currentlyActive = false;
	//flexible = false;

	leftSide  = new Point(x,     y + h/2);
	rightSide = new Point(x + w, y + h/2);
	
	lengthOfBolt = 4; //TODO: figure out logic for constraining this to make it look pretty
	bolts.reserve(maxBolts);
	int boltPoints = getDefaultNumBoltPoints(w);
	for (int i = 0; i < maxBolts; i++) {
		bolts.push_back(new LightningBolt(0, .5, 1, .5, boltPoints));
		simpleRefreshBolt(i);
	}

	canAcceptPowers = false;

	modifiesTankCollision = true;
	modifiesBulletCollision = true;

	local_initializeGPU();
	initializeGPU();
}

int HorizontalLightning::getDefaultNumBoltPoints(double horzDist) {
	int boltPoints = ceil(horzDist / lengthOfBolt); //not floor because the last point is the edge of the lightning area
	return (boltPoints < 2 ? 2 : boltPoints);
}

HorizontalLightning::~HorizontalLightning() {
	delete[] stateMultiplier;
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
	
	bolt_vb = new VertexBuffer(positions, bolts[0]->length*2 * sizeof(float), GL_STREAM_DRAW);
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
			//hazard tick comes before collision, therefore there will be another bolt refresh after this if a bullet/tank collides
			targetedTanks.clear();
			targetedBullets.clear();
			boltsNeeded = false;
			clearBolts();
			int boltPoints = getDefaultNumBoltPoints(w);
			for (int i = 0; i < maxBolts; i++) {
				bolts.push_back(new LightningBolt(0, .5, 1, .5, boltPoints));
				simpleRefreshBolt(i);
			}
			boltTick = 0;
		}
		boltTick++;
	} else {
		boltTick = boltCycle; //start at boltCycle to force a refresh as soon as possible
	}
}

void HorizontalLightning::modifiedTankCollision(Tank* t) {
	//if bolts are being randomized, clear them, and mark that they've been cleared
	if (!boltsNeeded) {
		clearBolts();
	}
	boltsNeeded = true;

	if (std::find(targetedTanks.begin(), targetedTanks.end(), t) == targetedTanks.end()) {
		targetedTanks.push_back(t);
	} else {
		return;
	}

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
	 *                x     x + t->r                     x+w - t->r  x+w
	 *
	 * 0 = leftStart/rightStart
	 * zone/region 7: circle radius t->r around 0
	 * determined by tank center
	 */

	//TODO: confirm everything is good
	double intersectionXL, intersectionXR, intersectionYL, intersectionYR;
	int boltPointsL = -1, boltPointsR = -1;
	//determine region the tank is in
	if (t->x-t->r >= x && t->x+t->r <= x+w) {
		if (t->y >= y && t->y <= y+h) {
			intersectionXL = t->x - t->r;
			intersectionXR = t->x + t->r;
			intersectionYL = t->y;
			intersectionYR = t->y;
			//std::cout << "case 1" << std::endl;
		} else {
			if (t->y > y+h) {
				intersectionYL = y + h;
				intersectionYR = y + h;
				//std::cout << "case 2a" << std::endl;
			} else { //t->y < y
				intersectionYL = y;
				intersectionYR = y;
				//std::cout << "case 2b" << std::endl;
			}
			intersectionXL = t->x - sqrt(pow(t->r, 2) - pow(intersectionYL - t->y, 2));
			//intersectionXL = (intersectionXL < x ? x : intersectionXL);
			intersectionXR = t->x + sqrt(pow(t->r, 2) - pow(intersectionYR - t->y, 2));
			//intersectionXR = (intersectionXR > x+w ? x+w : intersectionXR);
		}
	} else {
		//I think case 4 and 5 are the same
		if (t->x-t->r < x) {
			//left

			if (CollisionHandler::fullyCollided(leftSide, t)) {
				intersectionXL = t->x;
				intersectionYL = t->y;
				boltPointsL = 2;
				if (t->y > y+h) {
					intersectionXR = t->x + sqrt(pow(t->r, 2) - pow(t->y - (y+h), 2));
					intersectionYR = y + h;
				} else if(t->y < y) {
					intersectionXR = t->x + sqrt(pow(t->r, 2) - pow(t->y - y, 2));
					intersectionYR = y;
				} else {
					intersectionXR = t->x + t->r;
					intersectionYR = t->y;
				}
				//std::cout << "case 7L_L" << std::endl;
			} else {
				//check whether the tank is really far to the left, or just barely out of range of leftSide
				//difference between 6aL and 3aL: intersectionXL
				if (t->x <= x) {
					if (t->y > y+h) {
						intersectionXL = x;
						intersectionYL = t->y - sqrt(pow(t->r, 2) - pow(t->x - x, 2));
						intersectionXR = t->x + sqrt(pow(t->r, 2) - pow(t->y - (y+h), 2));
						intersectionYR = y + h;
						//std::cout << "case 6aL ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else if(t->y < y) {
						intersectionXL = x;
						intersectionYL = t->y + sqrt(pow(t->r, 2) - pow(t->x - x, 2));
						intersectionXR = t->x + sqrt(pow(t->r, 2) - pow(t->y - y, 2));
						intersectionYR = y;
						//std::cout << "case 6bL ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else {
						intersectionXL = x;
						if (t->y > y + h/2) {
							intersectionYL = t->y - sqrt(pow(t->r, 2) - pow(t->x - x, 2));
						} else {
							intersectionYL = t->y + sqrt(pow(t->r, 2) - pow(t->x - x, 2));
						}
						intersectionXR = t->x + t->r;
						intersectionYR = t->y;
						//std::cout << "case 5L ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					}
				} else {
					if (t->y > y+h) {
						intersectionXL = t->x - sqrt(pow(t->r, 2) - pow(t->y - (y+h), 2));
						intersectionXL = (intersectionXL < x ? x : intersectionXL);
						intersectionYL = t->y - sqrt(pow(t->r, 2) - pow(t->x - x, 2));
						intersectionYL = (intersectionYL > y+h ? y+h : intersectionYL);
						intersectionXR = t->x + sqrt(pow(t->r, 2) - pow(t->y - (y+h), 2));
						intersectionYR = y + h;
						//std::cout << "case 3aL ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else if(t->y < y) {
						intersectionXL = t->x - sqrt(pow(t->r, 2) - pow(t->y - y, 2));
						intersectionXL = (intersectionXL < x ? x : intersectionXL);
						intersectionYL = t->y + sqrt(pow(t->r, 2) - pow(t->x - x, 2));
						intersectionYL = (intersectionYL < y ? y : intersectionYL);
						intersectionXR = t->x + sqrt(pow(t->r, 2) - pow(t->y - y, 2));
						intersectionYR = y;
						//std::cout << "case 3bL ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else {
						intersectionXL = x;
						if (t->y > y + h/2) {
							intersectionYL = t->y - sqrt(pow(t->r, 2) - pow(t->x - x, 2));
						} else {
							intersectionYL = t->y + sqrt(pow(t->r, 2) - pow(t->x - x, 2));
						}
						intersectionXR = t->x + t->r;
						intersectionYR = t->y;
						//std::cout << "case 4L ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					}
				}
			}

			if (CollisionHandler::fullyCollided(rightSide, t)) { //this should be very rare (only happens for small lightnings)
				intersectionXR = t->x;
				intersectionYR = t->y;
				boltPointsR = 2;
				//std::cout << "case 7R_L" << std::endl;
			}
		} else { //t->x+t->r > x+w
			//right

			if (CollisionHandler::fullyCollided(rightSide, t)) {
				intersectionXR = t->x;
				intersectionYR = t->y;
				boltPointsR = 2;
				if (t->y > y+h) {
					intersectionXL = t->x - sqrt(pow(t->r, 2) - pow(t->y - (y+h), 2));
					intersectionYL = y + h;
				} else if(t->y < y) {
					intersectionXL = t->x - sqrt(pow(t->r, 2) - pow(t->y - y, 2));
					intersectionYL = y;
				} else {
					intersectionXL = t->x - t->r;
					intersectionYL = t->y;
				}
				//std::cout << "case 7R_R" << std::endl;
			} else {
				if (t->x >= x+w) {
					if (t->y > y+h) {
						intersectionXL = t->x - sqrt(pow(t->r, 2) - pow(t->y - (y+h), 2));
						intersectionYL = y + h;
						intersectionXR = x + w;
						intersectionYR = t->y - sqrt(pow(t->r, 2) - pow(t->x - (x+w), 2));
						//std::cout << "case 6aR ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else if(t->y < y) {
						intersectionXL = t->x - sqrt(pow(t->r, 2) - pow(t->y - y, 2));
						intersectionYL = y;
						intersectionXR = x + w;
						intersectionYR = t->y + sqrt(pow(t->r, 2) - pow(t->x - (x+w), 2));
						//std::cout << "case 6bR ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else {
						intersectionXL = t->x - t->r;
						intersectionYL = t->y;
						intersectionXR = x + w;
						if (t->y > y + h/2) {
							intersectionYR = t->y - sqrt(pow(t->r, 2) - pow(t->x - (x+w), 2));
						} else {
							intersectionYR = t->y + sqrt(pow(t->r, 2) - pow(t->x - (x+w), 2));
						}
						//std::cout << "case 5R ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					}
				} else {
					if (t->y > y+h) {
						intersectionXL = t->x - sqrt(pow(t->r, 2) - pow(t->y - (y+h), 2));
						intersectionYL = y + h;
						intersectionXR = t->x + sqrt(pow(t->r, 2) - pow(t->y - (y+h), 2));
						intersectionXR = (intersectionXR > x+w ? x+w : intersectionXR);
						intersectionYR = t->y - sqrt(pow(t->r, 2) - pow(t->x - (x+w), 2));
						intersectionYR = (intersectionYR > y+h ? y+h : intersectionYR);
						//std::cout << "case 3aR ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else if(t->y < y) {
						intersectionXL = t->x - sqrt(pow(t->r, 2) - pow(t->y - y, 2));
						intersectionYL = y;
						intersectionXR = t->x + sqrt(pow(t->r, 2) - pow(t->y - y, 2));
						intersectionXR = (intersectionXR > x+w ? x+w : intersectionXR);
						intersectionYR = t->y + sqrt(pow(t->r, 2) - pow(t->x - (x+w), 2));
						intersectionYR = (intersectionYR < y ? y : intersectionYR);
						//std::cout << "case 3bR ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					} else {
						intersectionXL = t->x - t->r;
						intersectionYL = t->y;
						intersectionXR = x + w;
						if (t->y > y + h/2) {
							intersectionYR = t->y - sqrt(pow(t->r, 2) - pow(t->x - (x+w), 2));
						} else {
							intersectionYR = t->y + sqrt(pow(t->r, 2) - pow(t->x - (x+w), 2));
						}
						//std::cout << "case 4R ";
						//std::cout << "xL:" << (intersectionXL - x) << " xR:" << (intersectionXR - x) << " yL:" << (intersectionYL - y) << " yR:" << (intersectionYR - y) << std::endl;
					}
				}
			}

			if (CollisionHandler::fullyCollided(leftSide, t)) { //this should be very rare (only happens for small lightnings)
				intersectionXL = t->x;
				intersectionYL = t->y;
				boltPointsL = 2;
				//std::cout << "case 7L_R" << std::endl;
			}
		}
	}
	double distL = sqrt(pow(intersectionXL - x, 2) + pow(intersectionYL - (y + h/2), 2));
	double distR = sqrt(pow((x + w) - intersectionXR, 2) + pow((y + h/2) - intersectionYR, 2));
	boltPointsL = (boltPointsL < 2 ? getDefaultNumBoltPoints(distL) : boltPointsL);
	boltPointsR = (boltPointsR < 2 ? getDefaultNumBoltPoints(distR) : boltPointsR);
	bolts.push_back(new LightningBolt(0, .5, (intersectionXL-x)/w, (intersectionYL-y)/h, boltPointsL));
	bolts.push_back(new LightningBolt((intersectionXL-x)/w, (intersectionYL-y)/h, (intersectionXR-x)/w, (intersectionYR-y)/h, 2));
	bolts.push_back(new LightningBolt((intersectionXR-x)/w, (intersectionYR-y)/h, 1, .5, boltPointsR));
	//simpleRefreshBolt(bolts.size()-2); simpleRefreshBolt(bolts.size()-1);

	//compared to JS Tanks, this intersection logic is much more complex
	//however, refreshing the bolts doesn't consider the horizontal edges (because this is a "horizontal lightning", so there's usually no need)
	//which means the performance hit isn't awful

	//TODO: is this complete?
}

void HorizontalLightning::modifiedBulletCollision(Bullet* b) {
	//TODO: add lightning bolts to the location
	return;
}

bool HorizontalLightning::validLocation() {
	//TODO: finish
	return true;

	/*
	var wallOnLeft=false, wallOnRight=false, wallInMiddle=false;
	for(var i=0; i<walls.length; i++){
		if(!(wallOnLeft && wallOnRight) && walls[i].ypos<=this.ypos && walls[i].ypos+walls[i].height>=this.ypos+this.height){
			if(!wallOnLeft && this.xpos==walls[i].xpos+walls[i].width)
				wallOnLeft=true;
			else if(!wallOnRight && this.xpos+this.width==walls[i].xpos)
				wallOnRight=true;
		}
		if(this.xpos+this.width>walls[i].xpos && this.xpos<walls[i].xpos+walls[i].width && this.ypos+this.height>walls[i].ypos && this.ypos<walls[i].ypos+walls[i].height){
			wallInMiddle=true;
			break;
		}
	}
	return (wallOnLeft && wallOnRight && !wallInMiddle);
	*/
}

void HorizontalLightning::simpleRefreshBolt(int num) {
	//double maxVarience = 4*h/w;
	double maxVarience = h/4;
	for (int j = 1; j < bolts[num]->length-1; j++) {
		//bolts[num]->positions[j*2]   = float(j)/(bolts[num]->length - 1);
		bolts[num]->positions[j*2+1] = rand()/double(RAND_MAX);
	}
}

void HorizontalLightning::refreshBolts() {
	for (int i = 0; i < bolts.size(); i++) {
		refreshBolt(i);
	}
}

void HorizontalLightning::refreshBolt(int num) {
	//double maxVarience = 4*h/w;
	double maxVarience = h/4;
	for (int j = 1; j < bolts[num]->length-1; j++) {
		bolts[num]->positions[j*2]   = float(j)/(bolts[num]->length - 1);
		bolts[num]->positions[j*2+1] = rand()/double(RAND_MAX);
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
	//shader->setUniformMat4f("u_MVP", MVPM);

	for (int i = 0; i < bolts.size(); i++) {
		streamBoltVertices(i);
		Renderer::Draw(*bolt_va, *shader, GL_LINE_STRIP, 0, bolts[i]->length);
	}
}

void HorizontalLightning::drawCPU() {
	//background:

	//bolts:

}