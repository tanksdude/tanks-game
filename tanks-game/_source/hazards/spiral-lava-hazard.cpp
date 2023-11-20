#include "spiral-lava-hazard.h"

#include "../constants.h"
#include <cmath>
#include <algorithm> //std::clamp, std::min
#include <iostream>
#include "../rng.h"

#include "../renderer.h"
#include "../color-mixer.h"
#include "../background-rect.h"

#include "../collision-handler.h"
#include "../tank-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"
#include "../hazard-data-governor.h"
#include "../level-manager.h"
#include "circular-lava-hazard.h"

std::unordered_map<std::string, float> SpiralLavaHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 0.5f });
	weights.insert({ "random-vanilla", 0.25f });
	weights.insert({ "random", 0.125f });
	return weights;
}

SpiralLavaHazard::SpiralLavaHazard(double xpos, double ypos, double width, int count, double dist, double rotateAngle) : RectHazard(HAZARD_TEAM) {
	x = xpos;
	y = ypos;
	w = h = width;
	if (LevelManager::getNumLevels() > 0) [[likely]] {
		color = LevelManager::getLevel(0)->getDefaultColor();
		//TODO: should this really be the same color as walls? feels a bit evil
	} else {
		color = ColorValueHolder(0, 0, 0);
	}

	maxLavaBlobs = count;
	lavaBlobIDs = std::vector<Game_ID>(maxLavaBlobs, -1);

	tickCount = 0;
	tickCycle = 200;
	currentlyActive = false; //true?
	moveClockwise = true;
	maxLavaDist = dist;
	lavaAngleRotate = rotateAngle;

	//canAcceptPowers = false;

	//future note: "challenges" should be a thing, and one challenge is to touch this hazard (with a short cycle) and get away in time
}

SpiralLavaHazard::SpiralLavaHazard(double xpos, double ypos, double width, int count) : SpiralLavaHazard(xpos, ypos, width, count, 80, PI) {}

SpiralLavaHazard::SpiralLavaHazard(double xpos, double ypos, double width) : SpiralLavaHazard(xpos, ypos, width, 16) {}

SpiralLavaHazard::~SpiralLavaHazard() {
	//nothing
}

RectHazard* SpiralLavaHazard::factory(const GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) [[likely]] {
		int count = args.getDataPortionLength(0);

		if (count >= 3) [[likely]] {
			const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
			double x = arr[0];
			double y = arr[1];
			double w = arr[2];

			if ((args.getDataCount() >= 2) && (args.getDataPortionLength(1) >= 1)) {
				int lavaCount = static_cast<const int*>(args.getDataPortion(1).get())[0];

				if ((args.getDataCount() >= 3) && (args.getDataPortionLength(2) >= 2)) {
					const double* arr_extra = static_cast<const double*>(args.getDataPortion(2).get());
					return new SpiralLavaHazard(x, y, w, lavaCount, arr_extra[0], arr_extra[1]);
				}

				return new SpiralLavaHazard(x, y, w, lavaCount);
			}

			return new SpiralLavaHazard(x, y, w);
		}
	}

	return new SpiralLavaHazard(0, 0, 0, 0);
}

void SpiralLavaHazard::initialize() {
	for (int i = 0; i < maxLavaBlobs; i++) {
		pushLavaBlob(i);
	}
	//TODO: this should delete its lava blobs when it dies
}

CircleHazard* SpiralLavaHazard::makeLavaBlob(int blobNum) const {
	GenericFactoryConstructionData constructionData;
	double* posArr = new double[3]{ this->x + this->w/2, this->y + this->h/2, getLavaBlobRadius() };
	constructionData = GenericFactoryConstructionData(3, posArr);
	CircleHazard* lavaBlob = HazardDataGovernor::getCircleHazardFactory("vanilla", "lava")(constructionData);
	return lavaBlob;
}

void SpiralLavaHazard::pushLavaBlob(int blobNum) {
	CircleHazard* lavaBlob = makeLavaBlob(blobNum);
	lavaBlobIDs[blobNum] = lavaBlob->getGameID();
	HazardManager::pushCircleHazard(lavaBlob);
}

inline double SpiralLavaHazard::getLavaBlobAngle(int blobNum, double tickValue) const {
	return (2*PI) * (blobNum / double(maxLavaBlobs)) + (tickValue / tickCycle) * (lavaAngleRotate * (moveClockwise ? 1 : -1));
}

inline double SpiralLavaHazard::getLavaBlobDist(double tickValue) const {
	return maxLavaDist * sin(PI * (tickValue / tickCycle)); //not 2*PI because only the >0 part is wanted
}

inline double SpiralLavaHazard::getLavaBlobRadius() const {
	return std::min(this->w, this->h) / 2; //in case it's not a square
}

bool SpiralLavaHazard::canReachTanks() const {
	const double reachDist = maxLavaDist + getLavaBlobRadius();
	Circle* testCircle = new Circle;
	testCircle->x = this->x + this->w/2;
	testCircle->y = this->y + this->h/2;
	testCircle->r = reachDist;
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		const Tank* t = TankManager::getTank(i);
		if (CollisionHandler::partiallyCollided(testCircle, t)) {
			delete testCircle;
			return true;
		}
	}
	delete testCircle;
	return false;
}

void SpiralLavaHazard::tick() {
	tickCount++;
	if (tickCount >= tickCycle) [[unlikely]] {
		//don't bother checking tickCycle <= 0
		tickCount -= tickCycle;
		currentlyActive = !currentlyActive;
		if (currentlyActive) {
			moveClockwise = !moveClockwise;
		}
	}

	const double tickValue = currentlyActive ? tickCount : 0;

	for (int i = 0; i < lavaBlobIDs.size(); i++) {
		CircleHazard* ch = HazardManager::getCircleHazardByID(lavaBlobIDs[i]);
		if (ch == nullptr) [[unlikely]] {
			//don't erase because the blob's position ID would get changed
			continue;
		}

		CircularLavaHazard* blob = static_cast<CircularLavaHazard*>(ch);
		const double lavaDist = getLavaBlobDist(tickValue);
		const double lavaAngle = getLavaBlobAngle(i, tickValue);
		blob->x = (this->x + this->w/2) + lavaDist * cos(lavaAngle);
		blob->y = (this->y + this->h/2) + lavaDist * sin(lavaAngle);
		//I would prefer to use deltas instead of absolutes, but this is way easier
		if (tickValue == 0) {
			//hack to allow touching the hazard without dying
			blob->r = getLavaBlobRadius() / 2;
		} else {
			blob->r = getLavaBlobRadius();
		}
	}
}

bool SpiralLavaHazard::reasonableLocation() const {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		if (CollisionHandler::partiallyCollided(this, WallManager::getWall(i))) {
			return false;
		}
	}

	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		const CircleHazard* ch = HazardManager::getCircleHazard(i);
		if (CollisionHandler::partiallyCollided(this, ch)) {
			return false;
		}
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		const RectHazard* rh = HazardManager::getRectHazard(i);
		if (rh->getGameID() != this->getGameID()) [[unlikely]] {
			if (CollisionHandler::partiallyCollided(this, rh)) {
				return false;
			}
		}
	}

	if (canReachTanks()) {
		return false;
	}

	return validLocation();
}

void SpiralLavaHazard::draw() const {
	ghostDraw(1.0f);
}

void SpiralLavaHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for " + getName() + " draw!" << std::endl;
			[[fallthrough]];
		case DrawingLayers::normal:
			draw();
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

void SpiralLavaHazard::poseDraw() const {
	draw();
}

void SpiralLavaHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for " + getName() + " poseDraw!" << std::endl;
			[[fallthrough]];
		case DrawingLayers::normal:
			poseDraw();
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

void SpiralLavaHazard::ghostDraw(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder c = this->color;
	c = ColorMixer::mix(BackgroundRect::getBackColor(), c, alpha);

	float coordsAndColor[] = {
		x,   y,     c.getRf(), c.getGf(), c.getBf(), c.getAf(),
		x+w, y,     c.getRf(), c.getGf(), c.getBf(), c.getAf(),
		x+w, y+h,   c.getRf(), c.getGf(), c.getBf(), c.getAf(),
		x,   y+h,   c.getRf(), c.getGf(), c.getBf(), c.getAf()
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	Renderer::SubmitBatchedDraw(coordsAndColor, 4 * (2+4), indices, 2 * 3);
}

void SpiralLavaHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for " + getName() + " ghostDraw!" << std::endl;
			[[fallthrough]];
		case DrawingLayers::normal:
			ghostDraw(alpha);
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

RectHazard* SpiralLavaHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData& args) {
	int attempts = 0;
	RectHazard* randomized = nullptr;
	double xpos, ypos, width;

	bool randomizeW;
	int count = 0;
	if (args.getDataCount() >= 1) {
		count = args.getDataPortionLength(0);
	}
	if (count >= 1) {
		const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
		width = arr[0];
		randomizeW = false;
	} else {
		randomizeW = true;
	}

	do {
		if (randomizeW) {
			width = RNG::randNumInRange(16, 24); //TODO: where should these constants be?
		}
		const double realRadius = 80 + width/2; //center of this to furthest reach of lava blobs
		xpos = RNG::randNumInRange(x_start + (realRadius-width/2), x_start + area_width - (realRadius+width/2));
		ypos = RNG::randNumInRange(y_start + (realRadius-width/2), y_start + area_height - (realRadius+width/2));
		RectHazard* testSpiralLava = new SpiralLavaHazard(xpos, ypos, width);
		if (testSpiralLava->reasonableLocation()) {
			randomized = testSpiralLava;
			break;
		} else {
			delete testSpiralLava;
		}
		attempts++;
	} while (attempts < 64);

	return randomized;
}
