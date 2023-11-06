#include "circular-lightning-hazard.h"

#include "../constants.h"
#include <cmath>
#include <stdexcept>
#include <algorithm> //std::copy, std::clamp
#include <iostream>
#include "../rng.h"
#include "../mylib.h" //pointInPolygon

#include "../renderer.h"
#include "../color-mixer.h"
#include "../background-rect.h"

#include "../collision-handler.h"
#include "../point.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"

SimpleVector2D CircularLightningHazard::body_vertices[Circle::numOfSides+1];
unsigned int CircularLightningHazard::body_indices[Circle::numOfSides*3];
unsigned int CircularLightningHazard::outline_indices[Circle::numOfSides*2*3];
bool CircularLightningHazard::initialized_vertices = false;

std::unordered_map<std::string, float> CircularLightningHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

CircularLightningHazard::CircularLightningHazard(double xpos, double ypos, double radius) : CircleHazard(HAZARD_TEAM) {
	x = xpos;
	y = ypos;
	r = radius;

	tickCount = 0;
	tickCycle = 100; //100 is JS default (because of power speed)
	double temp[2] = { 2, 2 };
	std::copy(temp, temp+2, stateMultiplier);
	currentlyActive = false;
	//flexible = false;

	maxBolts = 1;
	lengthOfBolt = 4; //JS: 2 (though the circular version didn't exist in JS)
	bolts.reserve(maxBolts);
	boltTick = 0;
	boltCycle = 4; //JS: none (though the circular version didn't exist in JS)
	boltsNeeded = false;

	//canAcceptPowers = false;

	modifiesTankCollision = true;
	hasSpecialEffectTankCollision = true;
	modifiesBulletCollision = true;
	hasSpecialEffectBulletCollision = true;

	initializeVertices();
}

inline Circle* CircularLightningHazard::getCenterPoint() const {
	return new Point(x, y);
}

CircularLightningHazard::~CircularLightningHazard() {
	//clearBolts(); //handled by ~GeneralizedLightning
}

bool CircularLightningHazard::initializeVertices() {
	if (initialized_vertices) [[likely]] {
		return false;
	}

	body_vertices[0] = SimpleVector2D(0, 0);
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		body_vertices[i] = SimpleVector2D(cos((i-1) * (2*PI / Circle::numOfSides)), sin((i-1) * (2*PI / Circle::numOfSides)));
	}

	for (int i = 0; i < Circle::numOfSides; i++) {
		body_indices[i*3]   = 0;
		body_indices[i*3+1] = i+1;
		body_indices[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	for (int i = 0; i < Circle::numOfSides; i++) {
		outline_indices[i*6]   =  i*2;
		outline_indices[i*6+1] =  i*2+1;
		outline_indices[i*6+2] = (i*2+3) % (Circle::numOfSides*2);
		outline_indices[i*6+3] = (i*2+3) % (Circle::numOfSides*2);
		outline_indices[i*6+4] = (i*2+2) % (Circle::numOfSides*2);
		outline_indices[i*6+5] =  i*2;
	}

	initialized_vertices = true;
	return true;
}

CircleHazard* CircularLightningHazard::factory(const GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) [[likely]] {
		int count = args.getDataPortionLength(0);

		if (count >= 3) [[likely]] {
			const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
			double x = arr[0];
			double y = arr[1];
			double r = arr[2];
			return new CircularLightningHazard(x, y, r);
		}
	}

	return new CircularLightningHazard(0, 0, 0);
}

void CircularLightningHazard::specialEffectCircleCollision(const Circle* c) {
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

void CircularLightningHazard::specialEffectTankCollision(const Tank* t) {
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

void CircularLightningHazard::specialEffectBulletCollision(const Bullet* b) {
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

void CircularLightningHazard::pushBolt(LightningBolt* l) {
	bolts.push_back(l);
	refreshBolt(l);
}

void CircularLightningHazard::pushDefaultBolt(int num, bool randomize) {
	//the default bolt is from center to a random point
	double randR = r*RNG::randFunc(), randAngle = (2*PI)*RNG::randFunc();
	double xEnd = randR*cos(randAngle), yEnd = randR*sin(randAngle);
	for (int i = 0; i < num; i++) {
		LightningBolt* l = new LightningBolt(0, 0, xEnd, yEnd, getDefaultNumBoltPoints(sqrt(pow(xEnd - 0, 2) + pow(yEnd - 0, 2))));
		if (randomize) {
			pushBolt(l);
		} else {
			bolts.push_back(l);
		}
	}
}

bool CircularLightningHazard::validLocation() const {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		const Wall* wa = WallManager::getWall(i);
		if (CollisionHandler::partiallyCollidedIgnoreEdge(wa, this)) {
			return false;
		}
	}
	return true;
}

bool CircularLightningHazard::reasonableLocation() const {
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		const CircleHazard* ch = HazardManager::getCircleHazard(i);
		if (ch->getName() != this->getName()) {
			if (CollisionHandler::partiallyCollided(this, ch)) {
				return false;
			}
		}
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		const RectHazard* rh = HazardManager::getRectHazard(i);
		if (rh->getName() != this->getName()) {
			if (CollisionHandler::partiallyCollided(this, rh)) {
				return false;
			}
		}
	}

	return validLocation();
}

void CircularLightningHazard::refreshBolt(LightningBolt* l) const {
	//TODO: more testing
	//see RectangularLightningHazard
	if (l->length <= 2) {
		return;
	}

	float deltaX = l->positions[l->length*2-2] - l->positions[0];
	float deltaY = l->positions[l->length*2-1] - l->positions[1];
	double dist = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
	double rotationAngle = atan2(deltaY, deltaX);
	double angleSin = sin(rotationAngle);
	double angleCos = cos(rotationAngle);
	double newH = dist * 1; //maybe *.5, I dunno
	double maxVariance = 1.0/4.0 * dist * 1; //(same here)

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
	//std::cout << "deltaY adj: " << (deltaY * 1/1) << std::endl;
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
			randTemp = (RNG::randFunc()*2-1)*maxVariance;
			testY = l->positions[j*2 - 1] + (deltaY/(l->length-1)) + randTemp * angleCos;
			testX = l->positions[j*2 - 2] + (deltaX/(l->length-1)) - randTemp * angleSin;
			//std::cout << testX << " " << testY << std::endl;
		} while (sqrt(pow(testY,2) + pow(testX,2)) > r || !pointInPolygon(6, polygonX, polygonY, testX, testY));
		//the first case is rare, but I'm fairly certain it's a useless check if pointInPolygon is checked first
		l->positions[j*2]   = testX;
		l->positions[j*2+1] = testY;
	}
}

void CircularLightningHazard::draw() const {
	drawBackground(false);
	drawBolts();
}

void CircularLightningHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			drawBackground(false);
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for " + getName() + " draw!" << std::endl;
			[[fallthrough]];
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

void CircularLightningHazard::poseDraw() const {
	drawBackground(true);
	drawBolts_Pose();
}

void CircularLightningHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			drawBackground(true);
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for " + getName() + " poseDraw!" << std::endl;
			[[fallthrough]];
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

void CircularLightningHazard::ghostDraw(float alpha) const {
	//not too sure on this
	drawBackground(true, alpha);
	drawBolts_Pose(alpha);
}

void CircularLightningHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			drawBackground(true, alpha);
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for " + getName() + " ghostDraw!" << std::endl;
			[[fallthrough]];
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

inline void CircularLightningHazard::drawBackground(bool pose, float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	double scale;
	if (pose || currentlyActive) {
		scale = 1.0;
	} else {
		scale = tickCount / (tickCycle * stateMultiplier[currentlyActive]);
	}
	scale = scale * scale;

	//ColorValueHolder color = (pose ? getBackgroundColor_Pose() : getBackgroundColor());
	ColorValueHolder color = getBackgroundColor_Pose();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

	float coordsAndColor[(Circle::numOfSides+1)*(2+4)];
	coordsAndColor[0] = x;
	coordsAndColor[1] = y;
	coordsAndColor[2] = color.getRf();
	coordsAndColor[3] = color.getGf();
	coordsAndColor[4] = color.getBf();
	coordsAndColor[5] = color.getAf();
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		coordsAndColor[i*6]   = x + (r*scale) * body_vertices[i].getXComp();
		coordsAndColor[i*6+1] = y + (r*scale) * body_vertices[i].getYComp();
		coordsAndColor[i*6+2] = color.getRf();
		coordsAndColor[i*6+3] = color.getGf();
		coordsAndColor[i*6+4] = color.getBf();
		coordsAndColor[i*6+5] = color.getAf();
	}

	Renderer::SubmitBatchedDraw(coordsAndColor, (Circle::numOfSides+1)*(2+4), body_indices, Circle::numOfSides*3);

	drawBackgroundOutline(alpha);
}

inline void CircularLightningHazard::drawBackgroundOutline(float alpha) const {
	//alpha set by drawBackground()

	//ColorValueHolder color_outline = ColorValueHolder(0.0f, 0.0f, 0.0f); //black is a bit too strong for a lightning's outline
	ColorValueHolder color_outline = ColorValueHolder(0.5f, 0.5f, 0.5f);
	color_outline = ColorMixer::mix(BackgroundRect::getBackColor(), color_outline, alpha);
	const float lineWidth = 0.5f;
	//using the same color for the background works well, though it's not used because the outline was added to make the lightning's boundary obvious

	float coordsAndColor_outline[(Circle::numOfSides*2)*(2+4)];
	for (int i = 0; i < Circle::numOfSides; i++) {
		coordsAndColor_outline[(i*2)  *6]   = x + (r-lineWidth) * body_vertices[i+1].getXComp();
		coordsAndColor_outline[(i*2)  *6+1] = y + (r-lineWidth) * body_vertices[i+1].getYComp();
		coordsAndColor_outline[(i*2+1)*6]   = x + (r+lineWidth) * body_vertices[i+1].getXComp();
		coordsAndColor_outline[(i*2+1)*6+1] = y + (r+lineWidth) * body_vertices[i+1].getYComp();

		coordsAndColor_outline[(i*2)  *6+2] = color_outline.getRf();
		coordsAndColor_outline[(i*2)  *6+3] = color_outline.getGf();
		coordsAndColor_outline[(i*2)  *6+4] = color_outline.getBf();
		coordsAndColor_outline[(i*2)  *6+5] = color_outline.getAf();
		coordsAndColor_outline[(i*2+1)*6+2] = color_outline.getRf();
		coordsAndColor_outline[(i*2+1)*6+3] = color_outline.getGf();
		coordsAndColor_outline[(i*2+1)*6+4] = color_outline.getBf();
		coordsAndColor_outline[(i*2+1)*6+5] = color_outline.getAf();
	}

	Renderer::SubmitBatchedDraw(coordsAndColor_outline, (Circle::numOfSides*2)*(2+4), outline_indices, Circle::numOfSides*6);
}

inline void CircularLightningHazard::drawBolts(float alpha) const {
	if (!currentlyActive) {
		return;
	}

	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color = getBoltColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	const float lineWidth = 0.75f;

	for (int i = 0; i < bolts.size(); i++) {
		float* coordsAndColor = new float[(bolts[i]->length-1)*4*(2+4)];
		unsigned int* indices = new unsigned int[(bolts[i]->length-1)*6];

		for (int j = 0; j < bolts[i]->length-1; j++) {
			const int startVertex = (j*4) * 6;
			const int startIndex = j*6;

			SimpleVector2D dist = SimpleVector2D(bolts[i]->positions[(j+1)*2] - bolts[i]->positions[j*2], bolts[i]->positions[(j+1)*2+1] - bolts[i]->positions[j*2+1]);
			SimpleVector2D distCW = SimpleVector2D(dist.getAngle() - PI/2, lineWidth, true);

			coordsAndColor[startVertex + 0*6]   = x + bolts[i]->positions[j*2]                     + distCW.getXComp();
			coordsAndColor[startVertex + 0*6+1] = y + bolts[i]->positions[j*2+1]                   + distCW.getYComp();
			coordsAndColor[startVertex + 1*6]   = x + bolts[i]->positions[j*2]   + dist.getXComp() + distCW.getXComp();
			coordsAndColor[startVertex + 1*6+1] = y + bolts[i]->positions[j*2+1] + dist.getYComp() + distCW.getYComp();
			coordsAndColor[startVertex + 2*6]   = x + bolts[i]->positions[j*2]   + dist.getXComp() - distCW.getXComp();
			coordsAndColor[startVertex + 2*6+1] = y + bolts[i]->positions[j*2+1] + dist.getYComp() - distCW.getYComp();
			coordsAndColor[startVertex + 3*6]   = x + bolts[i]->positions[j*2]                     - distCW.getXComp();
			coordsAndColor[startVertex + 3*6+1] = y + bolts[i]->positions[j*2+1]                   - distCW.getYComp();

			for (int k = 0; k < 4; k++) {
				coordsAndColor[startVertex + k*6+2] = color.getRf();
				coordsAndColor[startVertex + k*6+3] = color.getGf();
				coordsAndColor[startVertex + k*6+4] = color.getBf();
				coordsAndColor[startVertex + k*6+5] = color.getAf();
			}

			indices[startIndex + 0] = startVertex/6 + 0;
			indices[startIndex + 1] = startVertex/6 + 1;
			indices[startIndex + 2] = startVertex/6 + 2;
			indices[startIndex + 3] = startVertex/6 + 2;
			indices[startIndex + 4] = startVertex/6 + 3;
			indices[startIndex + 5] = startVertex/6 + 0;
		}

		Renderer::SubmitBatchedDraw(coordsAndColor, (bolts[i]->length-1)*4*(2+4), indices, (bolts[i]->length-1)*6);
		delete[] coordsAndColor; delete[] indices;
	}
}

inline void CircularLightningHazard::drawBolts_Pose(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	//generate bolts
	std::vector<LightningBolt*> poseBolts;
	for (int i = 0; i < 4; i++) {
		//from pushDefaultBolt(), mostly
		double dist = r * .75, angle = PI/4 + i*PI/2;
		double xEnd = dist*cos(angle), yEnd = dist*sin(angle);
		LightningBolt* l = new LightningBolt(0, 0, xEnd, yEnd, getDefaultNumBoltPoints(sqrt(pow(xEnd - 0, 2) + pow(yEnd - 0, 2))));

		refreshBolt(l);
		poseBolts.push_back(l);
	}

	//draw
	ColorValueHolder color = getBoltColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	const float lineWidth = 0.75f;

	for (int i = 0; i < poseBolts.size(); i++) {
		//match with drawBolts()
		float* coordsAndColor = new float[(poseBolts[i]->length-1)*4*(2+4)];
		unsigned int* indices = new unsigned int[(poseBolts[i]->length-1)*6];

		for (int j = 0; j < poseBolts[i]->length-1; j++) {
			const int startVertex = (j*4) * 6;
			const int startIndex = j*6;

			SimpleVector2D dist = SimpleVector2D(poseBolts[i]->positions[(j+1)*2] - poseBolts[i]->positions[j*2], poseBolts[i]->positions[(j+1)*2+1] - poseBolts[i]->positions[j*2+1]);
			SimpleVector2D distCW = SimpleVector2D(dist.getAngle() - PI/2, lineWidth, true);

			coordsAndColor[startVertex + 0*6]   = x + poseBolts[i]->positions[j*2]                     + distCW.getXComp();
			coordsAndColor[startVertex + 0*6+1] = y + poseBolts[i]->positions[j*2+1]                   + distCW.getYComp();
			coordsAndColor[startVertex + 1*6]   = x + poseBolts[i]->positions[j*2]   + dist.getXComp() + distCW.getXComp();
			coordsAndColor[startVertex + 1*6+1] = y + poseBolts[i]->positions[j*2+1] + dist.getYComp() + distCW.getYComp();
			coordsAndColor[startVertex + 2*6]   = x + poseBolts[i]->positions[j*2]   + dist.getXComp() - distCW.getXComp();
			coordsAndColor[startVertex + 2*6+1] = y + poseBolts[i]->positions[j*2+1] + dist.getYComp() - distCW.getYComp();
			coordsAndColor[startVertex + 3*6]   = x + poseBolts[i]->positions[j*2]                     - distCW.getXComp();
			coordsAndColor[startVertex + 3*6+1] = y + poseBolts[i]->positions[j*2+1]                   - distCW.getYComp();

			for (int k = 0; k < 4; k++) {
				coordsAndColor[startVertex + k*6+2] = color.getRf();
				coordsAndColor[startVertex + k*6+3] = color.getGf();
				coordsAndColor[startVertex + k*6+4] = color.getBf();
				coordsAndColor[startVertex + k*6+5] = color.getAf();
			}

			indices[startIndex + 0] = startVertex/6 + 0;
			indices[startIndex + 1] = startVertex/6 + 1;
			indices[startIndex + 2] = startVertex/6 + 2;
			indices[startIndex + 3] = startVertex/6 + 2;
			indices[startIndex + 4] = startVertex/6 + 3;
			indices[startIndex + 5] = startVertex/6 + 0;
		}

		Renderer::SubmitBatchedDraw(coordsAndColor, (poseBolts[i]->length-1)*4*(2+4), indices, (poseBolts[i]->length-1)*6);
		delete[] coordsAndColor; delete[] indices;
	}

	for (int i = 0; i < poseBolts.size(); i++) {
		delete poseBolts[i];
	}
}

CircleHazard* CircularLightningHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData& args) {
	int attempts = 0;
	CircleHazard* randomized = nullptr;
	double xpos, ypos, radius;

	bool randomizeR;
	int count = 0;
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);
	}
	if (count >= 1) {
		const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
		radius = arr[0];
		randomizeR = false;
	} else {
		randomizeR = true;
	}

	do {
		if (randomizeR) {
			radius = RNG::randFunc() * (60 - 30) + 30; //TODO: where should these constants be?
		}
		xpos = RNG::randFunc() * (area_width - 2*radius) + (x_start + radius);
		ypos = RNG::randFunc() * (area_height - 2*radius) + (y_start + radius);
		CircleHazard* testCircularLightning = new CircularLightningHazard(xpos, ypos, radius);
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
