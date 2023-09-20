#include "circular-no-bullet-zone-hazard.h"

#include "../constants.h"
#include <cmath>
#include <algorithm> //std::clamp
#include <iostream>
#include "../rng.h"

#include "../renderer.h"
#include "../color-mixer.h"
#include "../background-rect.h"

#include "../collision-handler.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"

std::unordered_map<std::string, float> CircularNoBulletZoneHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", .5f });
	weights.insert({ "random", .25f });
	return weights;
}

CircularNoBulletZoneHazard::CircularNoBulletZoneHazard(double xpos, double ypos, double radius) : CircleHazard(HAZARD_TEAM) {
	x = xpos;
	y = ypos;
	r = radius;

	//canAcceptPowers = false;

	modifiesTankCollision = true;
	modifiesBulletCollision = true;
}

CircularNoBulletZoneHazard::~CircularNoBulletZoneHazard() {
	//nothing
}

CircleHazard* CircularNoBulletZoneHazard::factory(const GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 3) {
			const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
			double x = arr[0];
			double y = arr[1];
			double r = arr[2];
			return new CircularNoBulletZoneHazard(x, y, r);
		}
	}
	return new CircularNoBulletZoneHazard(0, 0, 0);
}

bool CircularNoBulletZoneHazard::reasonableLocation() const {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		if (CollisionHandler::fullyCollided(this, WallManager::getWall(i))) {
			return false;
		}
	}

	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		CircleHazard* ch = HazardManager::getCircleHazard(i);
		if ((ch->getGameID() != this->getGameID()) && (ch->getName() != this->getName())) {
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

void CircularNoBulletZoneHazard::draw() const {
	ghostDraw(1.0f);
}

void CircularNoBulletZoneHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for CircularNoBulletZoneHazard::draw!" << std::endl;
		case DrawingLayers::under:
			draw();
			break;

		case DrawingLayers::normal:
			//nothing
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//nothing
			break;
	}
}

void CircularNoBulletZoneHazard::poseDraw() const {
	draw();
}

void CircularNoBulletZoneHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for CircularNoBulletZoneHazard::poseDraw!" << std::endl;
		case DrawingLayers::under:
			poseDraw();
			break;

		case DrawingLayers::normal:
			//nothing
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//nothing
			break;
	}
}

void CircularNoBulletZoneHazard::ghostDraw(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	//background:
	ColorValueHolder color_background = GeneralizedNoBulletZone::getColor();
	color_background = ColorMixer::mix(BackgroundRect::getBackColor(), color_background, alpha);

	float coordsAndColor_background[(Circle::numOfSides+1)*(2+4)];
	coordsAndColor_background[0] = x;
	coordsAndColor_background[1] = y;
	coordsAndColor_background[2] = color_background.getRf();
	coordsAndColor_background[3] = color_background.getGf();
	coordsAndColor_background[4] = color_background.getBf();
	coordsAndColor_background[5] = color_background.getAf();
	for (int i = 1; i < Circle::numOfSides+1; i++) {
		coordsAndColor_background[i*6]   = x + r * cos((i-1) * (2*PI / Circle::numOfSides));
		coordsAndColor_background[i*6+1] = y + r * sin((i-1) * (2*PI / Circle::numOfSides));
		coordsAndColor_background[i*6+2] = color_background.getRf();
		coordsAndColor_background[i*6+3] = color_background.getGf();
		coordsAndColor_background[i*6+4] = color_background.getBf();
		coordsAndColor_background[i*6+5] = color_background.getAf();
	}

	unsigned int indices_background[Circle::numOfSides*3];
	for (int i = 0; i < Circle::numOfSides; i++) {
		indices_background[i*3]   = 0;
		indices_background[i*3+1] = i+1;
		indices_background[i*3+2] = (i+1) % Circle::numOfSides + 1;
	}

	Renderer::SubmitBatchedDraw(coordsAndColor_background, (Circle::numOfSides+1)*(2+4), indices_background, Circle::numOfSides*3);

	//red X:
	ColorValueHolder color_extra = X_COLOR;
	color_extra = ColorMixer::mix(BackgroundRect::getBackColor(), color_extra, .75); //TODO: why?
	color_extra = ColorMixer::mix(BackgroundRect::getBackColor(), color_extra, alpha);

	const int num_vertices_fromSlashCenter = floor(Circle::numOfSides/4 * X_WIDTH); //vertices from the center of a slash
	const int num_vertices_outside = (2*num_vertices_fromSlashCenter + 1) * 4; //all the vertices on the outside
	//float* coordsAndColor_extra = new float[(num_vertices_outside+1)*(2+4)];
	//unsigned int* indices_extra = new unsigned int[num_vertices_fromSlashCenter*3 + 4*3];
	std::vector<float> coordsAndColor_extra;
	std::vector<unsigned int> indices_extra;

	coordsAndColor_extra.push_back(x);
	coordsAndColor_extra.push_back(y);
	coordsAndColor_extra.push_back(color_extra.getRf());
	coordsAndColor_extra.push_back(color_extra.getGf());
	coordsAndColor_extra.push_back(color_extra.getBf());
	coordsAndColor_extra.push_back(color_extra.getAf());

	for (int k = 0; k < 4; k++) {
		for (int i = -1*num_vertices_fromSlashCenter; i <= num_vertices_fromSlashCenter; i++) {
			int val = (k*Circle::numOfSides/4 + Circle::numOfSides/8) + i;
			coordsAndColor_extra.push_back(x + r * cos(val * (2*PI / Circle::numOfSides)));
			coordsAndColor_extra.push_back(y + r * sin(val * (2*PI / Circle::numOfSides)));
			coordsAndColor_extra.push_back(color_extra.getRf());
			coordsAndColor_extra.push_back(color_extra.getGf());
			coordsAndColor_extra.push_back(color_extra.getBf());
			coordsAndColor_extra.push_back(color_extra.getAf());
		}

		for (int i = 0; i < num_vertices_outside/4-1; i++) {
			indices_extra.push_back(0);
			indices_extra.push_back(k*(num_vertices_outside/4) + i+1);
			indices_extra.push_back(k*(num_vertices_outside/4) + i+2);
		}
	}

	indices_extra.push_back(0);
	indices_extra.push_back(3*(num_vertices_outside/4) + 0);
	indices_extra.push_back(0*(num_vertices_outside/4) + 1); //DL to UR, DR half
	indices_extra.push_back(0);
	indices_extra.push_back(1*(num_vertices_outside/4) + 0);
	indices_extra.push_back(2*(num_vertices_outside/4) + 1); //DL to UR, UL half
	indices_extra.push_back(0);
	indices_extra.push_back(4*(num_vertices_outside/4) + 0);
	indices_extra.push_back(1*(num_vertices_outside/4) + 1); //UL to DR, UR half
	indices_extra.push_back(0);
	indices_extra.push_back(2*(num_vertices_outside/4) + 0);
	indices_extra.push_back(3*(num_vertices_outside/4) + 1); //UL to DR, DL half
	//these draw over each other but that's easier than adding more vertices

	Renderer::SubmitBatchedDraw(coordsAndColor_extra.data(), coordsAndColor_extra.size(), indices_extra.data(), indices_extra.size());
	//delete[] coordsAndColor_extra; delete[] indices_extra;
}

void CircularNoBulletZoneHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for CircularNoBulletZoneHazard::ghostDraw!" << std::endl;
		case DrawingLayers::under:
			ghostDraw(alpha);
			break;

		case DrawingLayers::normal:
			//nothing
			break;

		case DrawingLayers::effects:
			//nothing
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//nothing
			break;
	}
}

CircleHazard* CircularNoBulletZoneHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData& args) {
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
			radius = RNG::randFunc() * (20 - 10) + 10; //TODO: where should these constants be?
		}
		xpos = RNG::randFunc() * (area_width - 2*radius) + (x_start + radius);
		ypos = RNG::randFunc() * (area_height - 2*radius) + (y_start + radius);
		CircleHazard* testCircularNoBulletZone = new CircularNoBulletZoneHazard(xpos, ypos, radius);
		if (testCircularNoBulletZone->reasonableLocation()) {
			randomized = testCircularNoBulletZone;
			break;
		} else {
			delete testCircularNoBulletZone;
		}
		attempts++;
	} while (attempts < 64);

	return randomized;
}