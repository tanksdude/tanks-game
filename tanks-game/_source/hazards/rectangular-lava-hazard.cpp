#include "rectangular-lava-hazard.h"

#include "../constants.h"
#include <cmath>
#include <algorithm> //std::sort, std::clamp
#include <iostream>
#include "../rng.h"

#include "../renderer.h"
#include "../color-mixer.h"
#include "../background-rect.h"

#include "../collision-handler.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"

std::unordered_map<std::string, float> RectangularLavaHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

RectangularLavaHazard::RectangularLavaHazard(double xpos, double ypos, double width, double height) : RectHazard(HAZARD_TEAM) {
	x = xpos;
	y = ypos;
	w = width;
	h = height;

	tickCount = 0;
	tickCycle = 2400;
	bubbles.reserve(maxBubbles);
	bubbleChance = 1.0/400;

	//canAcceptPowers = false;

	modifiesTankCollision = true;
	modifiesBulletCollision = true;
}

RectangularLavaHazard::~RectangularLavaHazard() {
	//nothing
}

RectHazard* RectangularLavaHazard::factory(const GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 4) {
			const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
			double x = arr[0];
			double y = arr[1];
			double w = arr[2];
			double h = arr[3];
			return new RectangularLavaHazard(x, y, w, h);
		}
	}
	return new RectangularLavaHazard(0, 0, 0, 0);
}

void RectangularLavaHazard::pushNewBubble(double radius) {
	float x0, y0, x1, y1;
	int attempts = 0;

	x0 = RNG::randFunc() * (w - radius*2) + radius;
	y0 = RNG::randFunc() * (h - radius*2) + radius;
	do {
		x1 = RNG::randFunc() * (w - radius*2) + radius;
		y1 = RNG::randFunc() * (h - radius*2) + radius;
		attempts++;
	} while ((attempts < 8) && (abs(x0-x1) < w/16 || abs(y0-y1) < h/16)); //JS Tanks used w/8 and h/8

	if (attempts < 8) {
		double maxTick = floor(RNG::randFunc()*101) + 200;
		bubbles.push_back(new LavaBubble(radius, x0/w, y0/h, x1/w, y1/h, maxTick));
	}
}

bool RectangularLavaHazard::reasonableLocation() const {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		if (CollisionHandler::partiallyCollided(this, WallManager::getWall(i))) {
			return false;
		}
	}

	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		if (CollisionHandler::partiallyCollided(this, HazardManager::getCircleHazard(i))) {
			return false;
		}
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		RectHazard* rh = HazardManager::getRectHazard(i);
		if ((rh->getGameID() != this->getGameID()) && (rh->getName() != this->getName())) {
			if (CollisionHandler::partiallyCollided(this, rh)) {
				return false;
			}
		}
	}

	return validLocation();
}

void RectangularLavaHazard::draw() const {
	drawBackground(false);
	drawBubbles(false);
}

void RectangularLavaHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularLavaHazard::draw!" << std::endl;
		case DrawingLayers::under:
			drawBackground(false);
			break;

		case DrawingLayers::normal:
			drawBubbles(false);
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

void RectangularLavaHazard::poseDraw() const {
	drawBackground(true);
}

void RectangularLavaHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularLavaHazard::poseDraw!" << std::endl;
		case DrawingLayers::under:
			drawBackground(true);
			break;

		case DrawingLayers::normal:
			//drawBubbles(true);
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

void RectangularLavaHazard::ghostDraw(float alpha) const {
	drawBackground(false, alpha);
	//no bubbles
}

void RectangularLavaHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularLavaHazard::ghostDraw!" << std::endl;
		case DrawingLayers::under:
			drawBackground(false, alpha);
			break;

		case DrawingLayers::normal:
			//drawBubbles(false, alpha);
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

inline void RectangularLavaHazard::drawBackground(bool pose, float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color = (pose ? getBackgroundColor_Pose() : getBackgroundColor());
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);

	float coordsAndColor[] = {
		x,   y,     color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		x+w, y,     color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		x+w, y+h,   color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		x,   y+h,   color.getRf(), color.getGf(), color.getBf(), color.getAf()
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	Renderer::SubmitBatchedDraw(coordsAndColor, 4 * (2+4), indices, 2 * 3);
}

inline void RectangularLavaHazard::drawBubbles(bool pose, float alpha) const {
	if (bubbles.size() == 0) {
		return;
	}

	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	//first, sort by alpha: lowest to highest (this makes the bubbles less weird-looking when drawn over each other)
	std::vector<LavaBubble*> sortedBubbles(bubbles);
	std::sort(sortedBubbles.begin(), sortedBubbles.end(),
		[](const LavaBubble* lhs, const LavaBubble* rhs) { return (lhs->getAlpha() < rhs->getAlpha()); });

	//second, draw the bubbles
	for (int j = 0; j < sortedBubbles.size(); j++) {
		ColorValueHolder color = (pose ? getBubbleColor_Pose(sortedBubbles[j]) : getBubbleColor(sortedBubbles[j]));
		color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
		const float lineWidth = 0.75f;

		float coordsAndColor[(Circle::numOfSides*2)*(2+4)];
		for (int i = 0; i < Circle::numOfSides; i++) {
			coordsAndColor[(i*2)  *6]   = (sortedBubbles[j]->getX()*this->w + this->x) + (sortedBubbles[j]->getR() - lineWidth) * cos(i * (2*PI / Circle::numOfSides));
			coordsAndColor[(i*2)  *6+1] = (sortedBubbles[j]->getY()*this->h + this->y) + (sortedBubbles[j]->getR() - lineWidth) * sin(i * (2*PI / Circle::numOfSides));
			coordsAndColor[(i*2+1)*6]   = (sortedBubbles[j]->getX()*this->w + this->x) + (sortedBubbles[j]->getR() + lineWidth) * cos(i * (2*PI / Circle::numOfSides));
			coordsAndColor[(i*2+1)*6+1] = (sortedBubbles[j]->getY()*this->h + this->y) + (sortedBubbles[j]->getR() + lineWidth) * sin(i * (2*PI / Circle::numOfSides));

			coordsAndColor[(i*2)  *6+2] = color.getRf();
			coordsAndColor[(i*2)  *6+3] = color.getGf();
			coordsAndColor[(i*2)  *6+4] = color.getBf();
			coordsAndColor[(i*2)  *6+5] = color.getAf();
			coordsAndColor[(i*2+1)*6+2] = color.getRf();
			coordsAndColor[(i*2+1)*6+3] = color.getGf();
			coordsAndColor[(i*2+1)*6+4] = color.getBf();
			coordsAndColor[(i*2+1)*6+5] = color.getAf();
		}

		unsigned int indices[Circle::numOfSides*6];
		for (int i = 0; i < Circle::numOfSides; i++) {
			indices[i*6]   =  i*2;
			indices[i*6+1] =  i*2+1;
			indices[i*6+2] = (i*2+3) % (Circle::numOfSides*2);
			indices[i*6+3] = (i*2+3) % (Circle::numOfSides*2);
			indices[i*6+4] = (i*2+2) % (Circle::numOfSides*2);
			indices[i*6+5] =  i*2;
		}

		Renderer::SubmitBatchedDraw(coordsAndColor, (Circle::numOfSides*2)*(2+4), indices, Circle::numOfSides*6);
	}
}

RectHazard* RectangularLavaHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData& args) {
	int attempts = 0;
	RectHazard* randomized = nullptr;
	double xpos, ypos, width, height;

	bool randomizeWH;
	int count = 0;
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);
	}
	if (count >= 2) {
		const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
		width = arr[0];
		height = arr[1];
		randomizeWH = false;
	} else {
		randomizeWH = true;
	}

	do {
		if (randomizeWH) {
			width = RNG::randFunc() * (120 - 30) + 30; //TODO: where should these constants be?
			height = RNG::randFunc() * (80 - 20) + 20; //TODO: where should these constants be?
		}
		xpos = RNG::randFunc() * (area_width - 2*width) + (x_start + width);
		ypos = RNG::randFunc() * (area_height - 2*height) + (y_start + height);
		RectHazard* testRectangularLava = new RectangularLavaHazard(xpos, ypos, width, height);
		if (testRectangularLava->reasonableLocation()) {
			randomized = testRectangularLava;
			break;
		} else {
			delete testRectangularLava;
		}
		attempts++;
	} while (attempts < 64);

	return randomized;
}
