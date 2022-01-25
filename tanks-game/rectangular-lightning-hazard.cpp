#include "rectangular-lightning-hazard.h"
#include "game-manager.h"
#include "renderer.h"
#include "background-rect.h"
#include "color-mixer.h"
#include "mylib.h"
#include "constants.h"
#include <math.h>
#include <stdexcept>
#include <algorithm> //std::copy
#include "point.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "collision-handler.h"
#include "rng.h"
#include <iostream>

VertexArray* RectangularLightningHazard::background_va;
VertexBuffer* RectangularLightningHazard::background_vb;
IndexBuffer* RectangularLightningHazard::background_ib;
bool RectangularLightningHazard::initialized_GPU = false;

std::unordered_map<std::string, float> RectangularLightningHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

RectangularLightningHazard::RectangularLightningHazard(double xpos, double ypos, double width, double height, bool) {
	x = xpos;
	y = ypos;
	w = width;
	h = height;
	gameID = GameManager::getNextID();
	teamID = HAZARD_TEAM;

	tickCount = 0;
	tickCycle = 100; //100 is JS default (because of power speed)
	double temp[2] = { 2, 2 };
	std::copy(temp, temp+2, stateMultiplier);
	currentlyActive = false;

	maxBolts = 1;
	lengthOfBolt = 4; //TODO: figure out logic for constraining this to make it look pretty
	bolts.reserve(maxBolts);
	boltTick = 0;
	boltCycle = 4;
	boltsNeeded = false;

	canAcceptPowers = false;

	modifiesTankCollision = true;
	hasSpecialEffectTankCollision = true;
	modifiesBulletCollision = true;
	hasSpecialEffectBulletCollision = true;
}

RectangularLightningHazard::RectangularLightningHazard(double xpos, double ypos, double width, double height)
: RectangularLightningHazard(xpos, ypos, width, height, true) {
	pushDefaultBolt(maxBolts, true); //there isn't really a default bolt...

	local_initializeGPU();
	initializeGPU();
}

inline Circle* RectangularLightningHazard::getCenterPoint() const {
	return new Point(x + w/2, y + h/2);
}

RectangularLightningHazard::~RectangularLightningHazard() {
	//clearBolts();

	local_uninitializeGPU();
	//uninitializeGPU();
}

bool RectangularLightningHazard::initializeGPU() {
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

	background_vb = VertexBuffer::MakeVertexBuffer(positions, 4*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout layout(2);
	background_va = VertexArray::MakeVertexArray(*background_vb, layout);

	background_ib = IndexBuffer::MakeIndexBuffer(indices, 6);

	initialized_GPU = true;
	return true;
}

//requires a bolt to initialize:
void RectangularLightningHazard::local_initializeGPU() {
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

void RectangularLightningHazard::local_reinitializeGPU(int length) { //does not seed the VertexBuffer with values
	delete bolt_va;
	delete bolt_vb;

	float* positions = new float[length*2];
	bolt_vb_length = length;

	bolt_vb = VertexBuffer::MakeVertexBuffer(positions, length*2 * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout(2);
	bolt_va = VertexArray::MakeVertexArray(*bolt_vb, layout);

	delete[] positions;
}

bool RectangularLightningHazard::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete background_va;
	delete background_vb;
	delete background_ib;

	initialized_GPU = false;
	return true;
}

void RectangularLightningHazard::local_uninitializeGPU() {
	delete bolt_va;
	delete bolt_vb;
}

void RectangularLightningHazard::streamBoltVertices(const LightningBolt* l) const {
	bolt_vb->modifyData(l->positions.data(), l->length*2 * sizeof(float));
}

RectHazard* RectangularLightningHazard::factory(GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 4) {
			double* arr = (double*)(args.getDataPortion(0));
			double x = arr[0];
			double y = arr[1];
			double w = arr[2];
			double h = arr[3];
			return new RectangularLightningHazard(x, y, w, h);
		}
	}
	return new RectangularLightningHazard(0, 0, 0, 0);
}

void RectangularLightningHazard::specialEffectCircleCollision(Circle* c) {
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
			//find distance from center to circle's center, minus circle's radius
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

			std::pair<PositionHolder, PositionHolder> intersections = CollisionHandler::circleLineIntersection(c, xpos, ypos, centerPoint->x, centerPoint->y);
			if (c->x < x + w/2) {
				intersectionX = std::max(intersections.first.x, intersections.second.x);
			} else {
				intersectionX = std::min(intersections.first.x, intersections.second.x);
			}
			if (c->y < y + h/2) {
				intersectionY = std::max(intersections.first.y, intersections.second.y);
			} else {
				intersectionY = std::min(intersections.first.y, intersections.second.y);
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

			//pushBolt(new LightningBolt(w/2, h/2, intersections.x1 - x, intersections.y1 - y, 2)); //debugging
			//pushBolt(new LightningBolt(w/2, h/2, intersections.x1 - x, intersections.y2 - y, 2));
			//pushBolt(new LightningBolt(w/2, h/2, intersections.x2 - x, intersections.y1 - y, 2));
			//pushBolt(new LightningBolt(w/2, h/2, intersections.x2 - x, intersections.y2 - y, 2));
		}
		delete circleCenter;
	}
	//double dist = sqrt(pow(intersectionX - centerPoint->x, 2) + pow(intersectionY - centerPoint->y, 2));
	//std::cout << dist << std::endl;
	//boltPoints = (boltPoints < 2 ? getDefaultNumBoltPoints(dist) : boltPoints); //no need
	pushBolt(new LightningBolt(w/2, h/2, intersectionX - x, intersectionY - y, boltPoints)); //TODO: is this right? (I think so)
	delete centerPoint;
}

void RectangularLightningHazard::specialEffectTankCollision(Tank* t) {
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

void RectangularLightningHazard::specialEffectBulletCollision(Bullet* b) {
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

void RectangularLightningHazard::pushBolt(LightningBolt* l) {
	if (l->length > bolt_vb_length) {
		local_reinitializeGPU(l->length);
	}
	bolts.push_back(l);
	refreshBolt(l);
}

void RectangularLightningHazard::pushDefaultBolt(int num, bool randomize) {
	//the default bolt is from center to a random point
	double xEnd = w*RNG::randFunc2(), yEnd = h*RNG::randFunc2();
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

bool RectangularLightningHazard::validLocation() const {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		if (CollisionHandler::partiallyCollidedIgnoreEdge(wa, this)) {
			return false;
		}
	}
	return true;
}

bool RectangularLightningHazard::reasonableLocation() const {
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		if (CollisionHandler::partiallyCollided(this, HazardManager::getCircleHazard(i))) {
			return false;
		}
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		RectHazard* rh = HazardManager::getRectHazard(i);
		if (rh->getGameID() != this->getGameID()) {
			//TODO: does this care if it's colliding with another version of itself?
			if (CollisionHandler::partiallyCollided(this, rh)) {
				return false;
			}
		}
	}

	return validLocation();
}

void RectangularLightningHazard::refreshBolt(LightningBolt* l) const {
	refreshBolt(l, std::min(h, w), std::max(h, w));
}

void RectangularLightningHazard::refreshBolt(LightningBolt* l, double smaller, double larger) const {
	//TODO: this needs more testing
	if (l->length <= 2) {
		return;
	}

	float deltaX = l->positions[l->length*2-2] - l->positions[0];
	float deltaY = l->positions[l->length*2-1] - l->positions[1];
	double dist = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
	double rotationAngle = atan2(deltaY, deltaX);
	double angleSin = sin(rotationAngle);
	double angleCos = cos(rotationAngle);
	double newH = dist * smaller/larger;
	double maxVariance = 1.0/4.0 * dist * smaller/larger;

	float polygonX[6] = {
		l->positions[0],
		l->positions[0] + deltaX * 1.0/4.0 - angleSin * newH * .5,
		l->positions[0] + deltaX * 3.0/4.0 - angleSin * newH * .5,
		l->positions[0] + deltaX,
		l->positions[0] + deltaX * 3.0/4.0 + angleSin * newH * .5,
		l->positions[0] + deltaX * 1.0/4.0 + angleSin * newH * .5
	};
	float polygonY[6] = {
		l->positions[1],
		l->positions[1] + deltaY * 1.0/4.0 + angleCos * newH * .5,
		l->positions[1] + deltaY * 3.0/4.0 + angleCos * newH * .5,
		l->positions[1] + deltaY,
		l->positions[1] + deltaY * 3.0/4.0 - angleCos * newH * .5,
		l->positions[1] + deltaY * 1.0/4.0 - angleCos * newH * .5
	};

	//std::cout << "deltaX: " << deltaX << std::endl;
	//std::cout << "deltaY: " << deltaY << std::endl;
	//std::cout << "deltaY adj: " << (deltaY * smaller/larger) << std::endl;
	//std::cout << "dist: " << dist << std::endl;
	//std::cout << "angle: " << (rotationAngle * 180/3.1415926535897) << std::endl;
	//std::cout << "cos(angle): " << angleCos << std::endl;
	//std::cout << "sin(angle): " << angleSin << std::endl;
	//for (int i = 0; i < 6; i++) {
	//	std::cout << i << ": " << polygonX[i] << " " << polygonY[i] << std::endl;
	//}

	for (int j = 1; j < l->length-1; j++) {
		double randTemp;
		float testY, testX;
		do {
			randTemp = (RNG::randFunc2()*2-1)*maxVariance;
			testY = l->positions[j*2 - 1] + (deltaY/(l->length-1)) + randTemp * angleCos;
			testX = l->positions[j*2 - 2] + (deltaX/(l->length-1)) - randTemp * angleSin;
			//std::cout << testX << " " << testY << std::endl;
		} while (testY < 0 || testY > h || testX < 0 || testX > w || !pointInPolygon(6, polygonX, polygonY, testX, testY));
		l->positions[j*2]   = testX;
		l->positions[j*2+1] = testY;
	}
}

void RectangularLightningHazard::draw() const {
	drawBackground(false);
	drawBolts();
}

void RectangularLightningHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			drawBackground(false);
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularLightningHazard::draw!" << std::endl;
		case DrawingLayers::normal:
			drawBolts();
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void RectangularLightningHazard::poseDraw() const {
	drawBackground(true);
	drawBolts_Pose();
}

void RectangularLightningHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			drawBackground(true);
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularLightningHazard::poseDraw!" << std::endl;
		case DrawingLayers::normal:
			drawBolts_Pose();
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void RectangularLightningHazard::ghostDraw(float alpha) const {
	drawBackground(true, alpha);
	drawBolts_Pose(alpha);
}

void RectangularLightningHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			drawBackground(true, alpha);
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularLightningHazard::ghostDraw!" << std::endl;
		case DrawingLayers::normal:
			drawBolts_Pose(alpha);
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

inline void RectangularLightningHazard::drawBackground(bool pose, float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	ColorValueHolder color = (pose ? getBackgroundColor_Pose() : getBackgroundColor());
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(w, h, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*background_va, *background_ib, *shader);
}

inline void RectangularLightningHazard::drawBolts(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	if (!currentlyActive) {
		return;
	}

	Renderer::SetLineWidth(2.0f);

	ColorValueHolder color = getBoltColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
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
		streamBoltVertices(bolts[i]); //TODO: fix (but better)
		Renderer::Draw(*bolt_va, *shader, GL_LINE_STRIP, 0, bolts[i]->length);
	}

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void RectangularLightningHazard::drawBolts_Pose(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	Renderer::SetLineWidth(2.0f);

	ColorValueHolder color = getBoltColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(1, 1, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	//generate bolts
	std::vector<LightningBolt*> poseBolts;
	for (int i = 0; i < 4; i++) {
		//from pushDefaultBolt(), mostly
		double xEnd = (i%2==0) * w/2*.75, yEnd = (i/2==0) * h/2*.75;
		LightningBolt* l = new LightningBolt(w/2, h/2, xEnd, yEnd, getDefaultNumBoltPoints(sqrt(pow(xEnd - w/2, 2) + pow(yEnd - h/2, 2))));

		if (l->length > bolt_vb_length) {
			//cut off the parts that won't fit; shouldn't happen though
			l->length = bolt_vb_length;
		}
		refreshBolt(l);
	}

	//draw
	for (int i = 0; i < poseBolts.size(); i++) {
		//match with drawBolts()
		streamBoltVertices(poseBolts[i]);
		Renderer::Draw(*bolt_va, *shader, GL_LINE_STRIP, 0, poseBolts[i]->length);
	}

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

RectHazard* RectangularLightningHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, GenericFactoryConstructionData& args) {
	int attempts = 0;
	RectHazard* randomized = nullptr;
	double xpos, ypos, width, height;

	bool randomizeWH;
	int count = 0;
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);
	}
	if (count >= 2) {
		double* arr = (double*)(args.getDataPortion(0));
		width = arr[0];
		height = arr[1];
		randomizeWH = false;
	} else {
		randomizeWH = true;
	}

	do {
		if (randomizeWH) {
			width = RNG::randFunc2() * (80 - 30) + 30; //TODO: where should these constants be?
			height = RNG::randFunc2() * (80 - 30) + 30; //TODO: where should these constants be?
		}
		xpos = RNG::randFunc2() * (area_width - 2*width) + (x_start + width);
		ypos = RNG::randFunc2() * (area_height - 2*height) + (y_start + height);
		RectHazard* testRectangularLightning = new RectangularLightningHazard(xpos, ypos, width, height);
		if (testRectangularLightning->reasonableLocation()) {
			randomized = testRectangularLightning;
			break;
		} else {
			delete testRectangularLightning;
		}
		attempts++;
	} while (attempts < 32);

	return randomized;
}
