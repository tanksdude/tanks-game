#include "reflecktor-hazard.h"

#include "../constants.h"
#include <cmath>
#include <algorithm> //std::clamp
#include <iostream>
#include "../rng.h"

#include "../renderer.h"
#include "../color-mixer.h"
#include "../background-rect.h"

#include "../collision-handler.h"
#include "../tank-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"
//#include "../level-manager.h"

std::unordered_map<std::string, float> ReflecktorHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

ReflecktorHazard::ReflecktorHazard(double xpos, double ypos, double width, double height) : RectHazard(HAZARD_TEAM) {
	x = xpos;
	y = ypos;
	w = width;
	h = height;

	color = ColorValueHolder(.95f, .95f, .95f);

	//canAcceptPowers = false;

	modifiesTankCollision = true;
}

ReflecktorHazard::~ReflecktorHazard() {
	//nothing
}

RectHazard* ReflecktorHazard::factory(const GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) [[likely]] {
		int count = args.getDataPortionLength(0);

		if (count >= 4) [[likely]] {
			const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
			double x = arr[0];
			double y = arr[1];
			double w = arr[2];
			double h = arr[3];
			return new ReflecktorHazard(x, y, w, h);
		}
	}

	return new ReflecktorHazard(0, 0, 0, 0);
}

void ReflecktorHazard::modifiedTankCollision(Tank* t) {
	//copied from PowerFunctionHelper::superbounceGeneric()

	if (!CollisionHandler::partiallyCollided(t, this)) {
		return;
	}

	double t_xDelta, t_yDelta, t_angleDelta;

	if (t->y - this->y <= (this->h / this->w) * (t->x - this->x)) {
		if (t->y - (this->y + this->h) <= (-this->h / this->w) * (t->x - this->x)) { //bottom
			t_yDelta = -1 * ((t->y + t->r - this->y) * 2);
			t_angleDelta = -2 * t->velocity.getAngle();
			t_xDelta = 0;
		} else { //right
			t_xDelta = (this->x + this->w - (t->x - t->r)) * 2;
			t_angleDelta = PI - 2*t->velocity.getAngle();
			t_yDelta = 0;
		}
	} else {
		if (t->y - (this->y + this->h) <= (-this->h / this->w) * (t->x - this->x)) { //left
			t_xDelta = -1 * ((t->x + t->r - this->x) * 2);
			t_angleDelta = PI - 2*t->velocity.getAngle();
			t_yDelta = 0;
		} else { //top
			t_yDelta = (this->y + this->h - (t->y - t->r)) * 2;
			t_angleDelta = -2 * t->velocity.getAngle();
			t_xDelta = 0;
		}
	}

	t->x += t_xDelta;
	t->y += t_yDelta;
	t->velocity.changeAngle(t_angleDelta);
}

bool ReflecktorHazard::reasonableLocation() const {
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

	return validLocation();
}

void ReflecktorHazard::draw() const {
	ghostDraw(1.0f);
}

void ReflecktorHazard::draw(DrawingLayers layer) const {
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

void ReflecktorHazard::poseDraw() const {
	draw();
}

void ReflecktorHazard::poseDraw(DrawingLayers layer) const {
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

void ReflecktorHazard::ghostDraw(float alpha) const {
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

void ReflecktorHazard::ghostDraw(DrawingLayers layer, float alpha) const {
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

RectHazard* ReflecktorHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData& args) {
	int attempts = 0;
	RectHazard* randomized = nullptr;
	double width, height;

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
			width = RNG::randFunc() * (64 - 12) + 12; //from LevelHelper::makeNewRandomWall
			height = RNG::randFunc() * (96 - 8) + 8;
		}
		RectHazard* testReflecktor = new ReflecktorHazard(x_start + RNG::randFunc() * (area_width - width), y_start + RNG::randFunc() * (area_height - height), width, height);
		if (testReflecktor->reasonableLocation()) {
			randomized = testReflecktor;
			break;
		} else {
			delete testReflecktor;
		}
		attempts++;
	} while (attempts < 64);

	return randomized;
}
