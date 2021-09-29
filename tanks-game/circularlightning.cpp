#include "circularlightning.h"
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
#include "wallmanager.h"
#include "hazardmanager.h"
#include "collisionhandler.h"
#include "rng.h"
#include <iostream>

VertexArray* CircularLightning::background_va;
VertexBuffer* CircularLightning::background_vb;
IndexBuffer* CircularLightning::background_ib;
bool CircularLightning::initialized_GPU = false;

CircularLightning::CircularLightning(double xpos, double ypos, double radius) {
	x = xpos;
	y = ypos;
	r = radius;
	gameID = GameManager::getNextID();
	teamID = HAZARD_TEAM;

	//tickCount = 0;
	tickCycle = 100; //100 is JS default (because of power speed)
	double temp[2] = { 2, 2 };
	std::copy(temp, temp+2, stateMultiplier);
	currentlyActive = false;
	//flexible = false;
	
	maxBolts = 1;
	lengthOfBolt = 4;
	bolts.reserve(maxBolts);
	pushDefaultBolt(maxBolts, true); //there isn't really a default bolt...
	//boltTick = 0;
	//boltCycle = 4;
	//boltsNeeded = false;

	canAcceptPowers = false;

	modifiesTankCollision = true;
	hasSpecialEffectTankCollision = true;
	modifiesBulletCollision = true;
	hasSpecialEffectBulletCollision = true;

	local_initializeGPU();
	initializeGPU();
}

Circle* CircularLightning::getCenterPoint() {
	return new Point(x, y);
}

CircularLightning::~CircularLightning() {
	//clearBolts();

	local_uninitializeGPU();
	//uninitializeGPU();
}

bool CircularLightning::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	float positions[(Circle::numOfSides+1)*2];
	positions[0] = 0;
	positions[1] = 0;
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		positions[i*2]   = cos((i-1) * 2*PI / Circle::numOfSides);
		positions[i*2+1] = sin((i-1) * 2*PI / Circle::numOfSides);
	}

	unsigned int indices[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices[i*3]   = 0;
		indices[i*3+1] = i+1;
		indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	background_vb = VertexBuffer::MakeVertexBuffer(positions, (Circle::numOfSides+1)*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout layout(2);
	background_va = VertexArray::MakeVertexArray(*background_vb, layout);

	background_ib = IndexBuffer::MakeIndexBuffer(indices, Circle::numOfSides*3);

	initialized_GPU = true;
	return true;
}

//requires a bolt to initialize:
void CircularLightning::local_initializeGPU() {
	float* positions = new float[bolts[0]->length*2];
	for (int i = 0; i < bolts[0]->length; i++) {
		positions[i*2]   = bolts[0]->positions[i*2];
		positions[i*2+1] = bolts[0]->positions[i*2+1];
	}
	bolt_vb_length = bolts[0]->length;
	
	bolt_vb = VertexBuffer::MakeVertexBuffer(positions, bolts[0]->length*2 * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout(2);
	bolt_va = VertexArray::MakeVertexArray(*bolt_vb, layout);

	delete[] positions;
}

void CircularLightning::local_reinitializeGPU(int length) { //does not seed the VertexBuffer with values
	delete bolt_va;
	delete bolt_vb;

	float* positions = new float[length*2];
	bolt_vb_length = length;
	
	bolt_vb = VertexBuffer::MakeVertexBuffer(positions, length*2 * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout(2);
	bolt_va = VertexArray::MakeVertexArray(*bolt_vb, layout);

	delete[] positions;
}

bool CircularLightning::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete background_va;
	delete background_vb;
	delete background_ib;

	initialized_GPU = false;
	return true;
}

void CircularLightning::local_uninitializeGPU() {
	delete bolt_va;
	delete bolt_vb;
}

void CircularLightning::streamBoltVertices(unsigned int boltNum) const {
	bolt_vb->modifyData(bolts[boltNum]->positions.data(), bolts[boltNum]->length*2 * sizeof(float));
}

CircleHazard* CircularLightning::factory(int argc, std::string* argv) {
	if (argc >= 3) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double r = std::stod(argv[2]);
		return new CircularLightning(x, y, r);
	}
	return new CircularLightning(0, 0, 0);
}

void CircularLightning::specialEffectCircleCollision(Circle* c) {
	//it's so nice how simple this is
	Circle* centerPoint = getCenterPoint();
	double intersectionX, intersectionY;
	int boltPoints = -1;
	if (CollisionHandler::fullyCollided(centerPoint, c)) {
		intersectionX = c->x;
		intersectionY = c->y;
		boltPoints = 2;
	} else {
		//find angle from center to circle's center
		double angle = atan2(c->y - centerPoint->y, c->x - centerPoint->x);
		//find distance from center to center, minus circle's radius
		double d = sqrt(pow(c->x - centerPoint->x, 2) + pow(c->y - centerPoint->y, 2)) - c->r;
		if (d < 0) {
			throw std::domain_error("ERROR: distance on circular lightning is wrong!");
			//is is really a "domain error?"
		}
		intersectionX = centerPoint->x + cos(angle) * d;
		intersectionY = centerPoint->y + sin(angle) * d;
		boltPoints = getDefaultNumBoltPoints(d);
	}
	//double dist = sqrt(pow(intersectionX - centerPoint->x, 2) + pow(intersectionY - centerPoint->y, 2));
	//std::cout << dist << std::endl;
	//boltPoints = (boltPoints < 2 ? getDefaultNumBoltPoints(dist) : boltPoints); //no need
	pushBolt(new LightningBolt(0, 0, intersectionX - x, intersectionY - y, boltPoints)); //TODO: is this right? (probably)
	delete centerPoint;
}

void CircularLightning::specialEffectTankCollision(Tank* t) {
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

void CircularLightning::specialEffectBulletCollision(Bullet* b) {
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

void CircularLightning::pushBolt(LightningBolt* l) {
	if (l->length > bolt_vb_length) {
		local_reinitializeGPU(l->length);
	}
	bolts.push_back(l);
	refreshBolt(bolts.size() - 1);
}

void CircularLightning::pushDefaultBolt(int num, bool randomize) {
	//the default bolt is from center to a random point
	double randR = r*RNG::randFunc2(), randAngle = 2*PI*RNG::randFunc();
	double xEnd = randR*cos(randAngle), yEnd = randR*sin(randAngle);
	for (int i = 0; i < num; i++) {
		LightningBolt* l = new LightningBolt(0, 0, xEnd, yEnd, getDefaultNumBoltPoints(sqrt(pow(xEnd - 0, 2) + pow(yEnd - 0, 2))));
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

bool CircularLightning::validLocation() {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		if (CollisionHandler::partiallyCollidedIgnoreEdge(wa, this)) {
			return false;
		}
	}
	return true;
}

bool CircularLightning::reasonableLocation() {
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		CircleHazard* ch = HazardManager::getCircleHazard(i);
		if (ch->getGameID() != this->getGameID()) {
			//TODO: does this care if it's colliding with another version of itself?
			if (CollisionHandler::partiallyCollided(this, ch)) {
				return false;
			}
		}
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		if (CollisionHandler::partiallyCollided(this, HazardManager::getRectHazard(i))) {
			return false;
		}
	}

	return validLocation();
}

void CircularLightning::refreshBolt(int num) {
	//TODO: more testing
	//see RectangularLightning
	if (bolts[num]->length <= 2) {
		return;
	}

	float deltaX = bolts[num]->positions[bolts[num]->length*2-2] - bolts[num]->positions[0];
	float deltaY = bolts[num]->positions[bolts[num]->length*2-1] - bolts[num]->positions[1];
	double dist = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
	double rotationAngle = atan2(deltaY, deltaX);
	double angleSin = sin(rotationAngle);
	double angleCos = cos(rotationAngle);
	double newH = dist * 1; //maybe *.5, I dunno
	double maxVariance = 1.0/4.0 * dist * 1; //(same here)

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
	//std::cout << "deltaY adj: " << (deltaY * 1/1) << std::endl;
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
			randTemp = (RNG::randFunc2()*2-1)*maxVariance;
			testY = bolts[num]->positions[j*2 - 1] + (deltaY/(bolts[num]->length-1)) + randTemp * angleCos;
			testX = bolts[num]->positions[j*2 - 2] + (deltaX/(bolts[num]->length-1)) - randTemp * angleSin;
			//std::cout << testX << " " << testY << std::endl;
		} while (sqrt(pow(testY,2) + pow(testX,2)) > r || !pointInPolygon(6, polygonX, polygonY, testX, testY));
		//the first case is rare, but I'm fairly certain it's a useless check if pointInPolygon is checked first
		bolts[num]->positions[j*2]   = testX;
		bolts[num]->positions[j*2+1] = testY;
	}
}

void CircularLightning::draw() const {
	draw(x, y);
}

void CircularLightning::draw(double xpos, double ypos) const {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(r, r, 0, xpos, ypos);
	
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
	MVPM = Renderer::GenerateMatrix(1, 1, 0, xpos, ypos);
	shader->setUniformMat4f("u_MVP", MVPM);

	for (int i = 0; i < bolts.size(); i++) {
		//I think the VertexBuffer resizing should happen here, but there would probably be less strain if it happens only when a bullet/tank collides
		//TODO: that ^ should be the preferred way, since only draw() (and initializeGPU()) should do GPU stuff
		/*
		if (bolts[i]->length > bolt_vb_length) {
			local_reinitializeGPU(bolts[i]->length);
		}
		*/
		streamBoltVertices(i); //TODO: fix
		Renderer::Draw(*bolt_va, *shader, GL_LINE_STRIP, 0, bolts[i]->length);
	}
}

void CircularLightning::poseDraw() const {
	//TODO
	return;
}

CircleHazard* CircularLightning::randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv) {
	int attempts = 0;
	CircleHazard* randomized = nullptr;
	double xpos, ypos, radius;
	do {
		if (argc >= 1) {
			radius = std::stod(argv[0]);
		} else {
			radius = RNG::randFunc2() * (60 - 30) + 30; //TODO: where should these constants be?
		}
		xpos = RNG::randFunc2() * (area_width - 2*radius) + (x_start + radius);
		ypos = RNG::randFunc2() * (area_height - 2*radius) + (y_start + radius);
		CircleHazard* testCircularLightning = new CircularLightning(xpos, ypos, radius);
		if (testCircularLightning->reasonableLocation()) {
			randomized = testCircularLightning;
			break;
		} else {
			delete testCircularLightning;
		}
		attempts++;
	} while (attempts < 32);

	return randomized;
}
