#include "rectangularlightning.h"
#include "gamemanager.h"
#include "renderer.h"
#include "backgroundrect.h"
#include "colormixer.h"
#include "mylib.h"
#include "constants.h"
#include <math.h>
#include <stdexcept>
#include <algorithm> //std::copy
#include "point.h"
#include "collisionhandler.h"
#include "wallmanager.h"
#include <iostream>

VertexArray* RectangularLightning::background_va;
VertexBuffer* RectangularLightning::background_vb;
IndexBuffer* RectangularLightning::background_ib;
bool RectangularLightning::initialized_GPU = false;

RectangularLightning::RectangularLightning(double xpos, double ypos, double width, double height, bool) {
	x = xpos;
	y = ypos;
	w = width;
	h = height;
	gameID = GameManager::getNextID();
	teamID = HAZARD_TEAM;

	//tickCount = 0;
	tickCycle = 100; //100 is JS default (because of power speed)
	double temp[2] = { 2, 2 };
	std::copy(temp, temp+2, stateMultiplier);
	currentlyActive = false;
	//flexible = false;
	
	maxBolts = 1;
	lengthOfBolt = 4; //TODO: figure out logic for constraining this to make it look pretty
	bolts.reserve(maxBolts);
	//boltTick = 0;
	//boltCycle = 4;
	//boltsNeeded = false;

	canAcceptPowers = false;

	modifiesTankCollision = true;
	hasSpecialEffectTankCollision = true;
	modifiesBulletCollision = true;
	hasSpecialEffectBulletCollision = true;
}

RectangularLightning::RectangularLightning(double xpos, double ypos, double width, double height)
: RectangularLightning(xpos, ypos, width, height, true) {
	pushDefaultBolt(maxBolts, true); //there isn't really a default bolt...

	local_initializeGPU();
	initializeGPU();
}

Circle* RectangularLightning::getCenterPoint() {
	return new Point(x + w/2, y + h/2);
}

RectangularLightning::~RectangularLightning() {
	//clearBolts();

	local_uninitializeGPU();
	//uninitializeGPU();
}

bool RectangularLightning::initializeGPU() {
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

//requires a bolt to initialize:
void RectangularLightning::local_initializeGPU() {
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

void RectangularLightning::local_reinitializeGPU(int length) { //does not seed the VertexBuffer with values
	delete bolt_va;
	delete bolt_vb;

	float* positions = new float[length*2];
	bolt_vb_length = length;
	
	bolt_vb = new VertexBuffer(positions, length*2 * sizeof(float), GL_STREAM_DRAW);
	VertexBufferLayout layout(2);
	bolt_va = new VertexArray(*bolt_vb, layout);

	delete[] positions;
}

bool RectangularLightning::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete background_va;
	delete background_vb;
	delete background_ib;

	initialized_GPU = false;
	return true;
}

void RectangularLightning::local_uninitializeGPU() {
	delete bolt_va;
	delete bolt_vb;
}

void RectangularLightning::streamBoltVertices(unsigned int boltNum) {
	bolt_vb->modifyData(bolts[boltNum]->positions.data(), bolts[boltNum]->length*2 * sizeof(float));
}

RectHazard* RectangularLightning::factory(int argc, std::string* argv) {
	if (argc >= 4) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double w = std::stod(argv[2]);
		double h = std::stod(argv[3]);
		return new RectangularLightning(x, y, w, h);
	}
	return new RectangularLightning(0, 0, 0, 0);
}

void RectangularLightning::specialEffectCircleCollision(Circle* c) {
	//TODO: is this done?
	Circle* centerPoint = getCenterPoint();
	double intersectionX, intersectionY;
	int boltPoints = -1;
	if (CollisionHandler::fullyCollided(centerPoint, c)) {
		intersectionX = c->x;
		intersectionY = c->y;
		boltPoints = 2;
	} else {
		//if the circle's center isn't fully in the rectangle area, then the endpoint may not be inside the rectangle
		Circle* circleCenter = new Point(c->x, c->y);
		if (CollisionHandler::fullyCollided(circleCenter, this)) {
			//circle center inside rectangle area
			//find angle from center to circle's center
			double angle = atan2(c->y - centerPoint->y, c->x - centerPoint->x);
			//find distance from center to center, minus circle's radius
			double d = sqrt(pow(centerPoint->x - c->x, 2) + pow(centerPoint->y - c->y, 2)) - c->r;
			if (d < 0) {
				throw std::domain_error("ERROR: distance on rectangular lightning is wrong!");
				//is is really a "domain error?"
			}
			intersectionX = centerPoint->x + cos(angle) * d;
			intersectionY = centerPoint->y + sin(angle) * d;
			boltPoints = getDefaultNumBoltPoints(d);
		} else {
			//circle center outside rectangle area
			double xpos, ypos;
			if (c->x < x) {
				xpos = x; //circle too far to the left
			} else if (c->x > x+w) {
				xpos = x+w; //circle too far to the right
			} else {
				xpos = c->x; //circle's x-position is fine
			}
			if (c->y < y) {
				ypos = y; //circle too low
			} else if (c->y > y+h) {
				ypos = y+h; //circle too high
			} else {
				ypos = c->y; //circle's y-position is fine
			}
			//std::cout << "xpos: " << (xpos-c->x) << ", ypos: " << (ypos-c->y) << std::endl;
			//circle-line intersection: https://mathworld.wolfram.com/Circle-LineIntersection.html
			//normally I always use pow(x,2) for clarity, but it's getting repetitive, and also x*x is faster
			double x1 = xpos - c->x, x2 = centerPoint->x - c->x;
			double y1 = ypos - c->y, y2 = centerPoint->y - c->y;
			double dx = x2-x1, dy = y2-y1;
			double dr = sqrt(pow(dx,2) + pow(dy,2));
			double D = x1*y2 - x2*y1; //I spent about an hour debugging this, only to find out I did x1*y2 - x1*y1. Don't make the same mistake.
			double intersectionX1 = (D*dy - (dy<0 ? -1 : 1) * dx * sqrt(c->r*c->r * dr*dr - D*D)) / (dr*dr);
			double intersectionX2 = (D*dy + (dy<0 ? -1 : 1) * dx * sqrt(c->r*c->r * dr*dr - D*D)) / (dr*dr);
			double intersectionY1 = (-D*dx - abs(dy) * sqrt(c->r*c->r * dr*dr - D*D)) / (dr*dr);
			double intersectionY2 = (-D*dx + abs(dy) * sqrt(c->r*c->r * dr*dr - D*D)) / (dr*dr);
			//std::cout << "x1: " << intersectionX1 << ", ";
			//std::cout << "x2: " << intersectionX2 << ", ";
			//std::cout << "y1: " << intersectionY1 << ", ";
			//std::cout << "y2: " << intersectionY2 << std::endl;
			
			if (c->x < x + w/2) {
				intersectionX = std::max(intersectionX1, intersectionX2) + c->x;
			} else {
				intersectionX = std::min(intersectionX1, intersectionX2) + c->x;
			}
			if (c->y < y + h/2) {
				intersectionY = std::max(intersectionY1, intersectionY2) + c->y;
			} else {
				intersectionY = std::min(intersectionY1, intersectionY2) + c->y;
			}
			if (intersectionX < x || intersectionX > x+w) {
				std::cerr << "WARNING: rectangular lightning endpoint X out of range!" << std::endl;
				intersectionX = constrain<double>(intersectionX, x, x+w);
			}
			if (intersectionY < y || intersectionY > y+h) {
				std::cerr << "WARNING: rectangular lightning endpoint Y out of range!" << std::endl;
				intersectionY = constrain<double>(intersectionY, y, y+h);
			}
			boltPoints = getDefaultNumBoltPoints(sqrt(pow(intersectionX - centerPoint->x, 2) + pow(intersectionY - centerPoint->y, 2)));
			//pushBolt(new LightningBolt(w/2, h/2, intersectionX1 + c->x - x, intersectionY1 + c->y - y, 2)); //debugging
			//pushBolt(new LightningBolt(w/2, h/2, intersectionX1 + c->x - x, intersectionY2 + c->y - y, 2));
			//pushBolt(new LightningBolt(w/2, h/2, intersectionX2 + c->x - x, intersectionY1 + c->y - y, 2));
			//pushBolt(new LightningBolt(w/2, h/2, intersectionX2 + c->x - x, intersectionY2 + c->y - y, 2));
		}
		delete circleCenter;
	}
	//double dist = sqrt(pow(intersectionX - centerPoint->x, 2) + pow(intersectionY - centerPoint->y, 2));
	//std::cout << dist << std::endl;
	//boltPoints = (boltPoints < 2 ? getDefaultNumBoltPoints(dist) : boltPoints); //no need
	pushBolt(new LightningBolt(w/2, h/2, intersectionX - x, intersectionY - y, boltPoints)); //TODO: is this right? (I think so)
	delete centerPoint;
}

void RectangularLightning::specialEffectTankCollision(Tank* t) {
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

void RectangularLightning::specialEffectBulletCollision(Bullet* b) {
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

void RectangularLightning::pushBolt(LightningBolt* l) {
	if (l->length > bolt_vb_length) {
		local_reinitializeGPU(l->length);
	}
	bolts.push_back(l);
	refreshBolt(bolts.size() - 1);
}

void RectangularLightning::pushDefaultBolt(int num, bool randomize) {
	//the default bolt is from center to a random point
	double xEnd = w*randFunc2(), yEnd = h*randFunc2();
	for (int i = 0; i < num; i++) {
		LightningBolt* l = new LightningBolt(w/2, h/2, xEnd, yEnd, getDefaultNumBoltPoints(sqrt(pow(xEnd - w/2, 2) + pow(yEnd - h/2, 2))));
		if (randomize) {
			pushBolt(l);
		} else {
			if (l->length > bolt_vb_length) {
				local_reinitializeGPU(l->length);
			}
			bolts.push_back(l);
		}
	}
}

bool RectangularLightning::validLocation() {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		if (CollisionHandler::partiallyCollidedIgnoreEdge(wa, this)) {
			return false;
		}
	}
	return true;
}

bool RectangularLightning::reasonableLocation() {
	return validLocation();
}

void RectangularLightning::refreshBolt(int num) {
	//TODO: this needs more testing
	if (bolts[num]->length <= 2) {
		return;
	}

	double smaller = std::min(h, w), larger = std::max(h, w);
	float deltaX = bolts[num]->positions[bolts[num]->length*2-2] - bolts[num]->positions[0];
	float deltaY = bolts[num]->positions[bolts[num]->length*2-1] - bolts[num]->positions[1];
	double dist = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
	double rotationAngle = atan2(deltaY, deltaX);
	double angleSin = sin(rotationAngle);
	double angleCos = cos(rotationAngle);
	double newH = dist * smaller/larger;
	double maxVariance = 1.0/4.0 * dist * smaller/larger;

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

void RectangularLightning::draw() {
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
		//TODO: that ^ should be the preferred way, since only draw() (and initializeGPU()) should do GPU stuff
		/*
		if (bolts[i]->length > bolt_vb_length) {
			local_reinitializeGPU(bolts[i]->length);
		}
		*/
		streamBoltVertices(i);
		Renderer::Draw(*bolt_va, *shader, GL_LINE_STRIP, 0, bolts[i]->length);
	}
}

void RectangularLightning::drawCPU() {
	//background:

	//bolts:

}
