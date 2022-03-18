#include "dev-wall-hazard.h"
#include "renderer.h"
#include "constants.h"
#include <math.h>
#include "color-mixer.h"
#include "background-rect.h"
#include "mylib.h"
#include "point.h"
#include "tank.h"
#include "tank-manager.h"
#include "bullet-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "level-manager.h"
#include "collision-handler.h"
#include "rng.h"

VertexArray* DevWallHazard::va;
VertexBuffer* DevWallHazard::vb;
IndexBuffer* DevWallHazard::ib;
bool DevWallHazard::initialized_GPU = false;

std::unordered_map<std::string, float> DevWallHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

DevWallHazard::DevWallHazard(double xpos, double ypos, double width, double height) : RectHazard(DEFAULT_TEAM) {
	x = xpos;
	y = ypos;
	w = width;
	h = height;
	if (LevelManager::getNumLevels() > 0) {
		color = LevelManager::getLevel(0)->getDefaultColor();
	} else {
		color = ColorValueHolder(0, 0, 0);
	}

	canAcceptPowers = false;

	//modifiesBulletCollision = true;

	initializeGPU();
}

DevWallHazard::~DevWallHazard() {
	//uninitializeGPU();
}

bool DevWallHazard::initializeGPU() {
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

	vb = VertexBuffer::MakeVertexBuffer(positions, 4*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout layout(2);
	va = VertexArray::MakeVertexArray(*vb, layout);

	ib = IndexBuffer::MakeIndexBuffer(indices, 6);

	initialized_GPU = true;
	return true;
}

bool DevWallHazard::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;

	initialized_GPU = false;
	return true;
}

RectHazard* DevWallHazard::factory(GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 4) {
			double* arr = (double*)(args.getDataPortion(0));
			double x = arr[0];
			double y = arr[1];
			double w = arr[2];
			double h = arr[3];
			return new DevWallHazard(x, y, w, h);
		}
	}
	return new DevWallHazard(0, 0, 0, 0);
}

bool DevWallHazard::reasonableLocation() const {
	/*
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		if (CollisionHandler::partiallyCollided(this, WallManager::getWall(i))) {
			return false;
		}
	}
	*/

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

void DevWallHazard::draw() const {
	ghostDraw(1.0f);
}

void DevWallHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for DevWallHazard::draw!" << std::endl;
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

void DevWallHazard::poseDraw() const {
	draw();
}

void DevWallHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for DevWallHazard::poseDraw!" << std::endl;
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

void DevWallHazard::ghostDraw(float alpha) const {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	ColorValueHolder c = color;
	c = ColorMixer::mix(BackgroundRect::getBackColor(), c, alpha);
	shader->setUniform4f("u_color", c.getRf(), c.getGf(), c.getBf(), c.getAf());

	MVPM = Renderer::GenerateMatrix(w, h, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *ib, *shader);
}

void DevWallHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for DevWallHazard::ghostDraw!" << std::endl;
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

RectHazard* DevWallHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, GenericFactoryConstructionData& args) {
	int attempts = 0;
	RectHazard* randomized = nullptr;
	double width, height;

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
			width = RNG::randFunc() * (64 - 12) + 12; //from LevelHelper::makeNewRandomWall
			height = RNG::randFunc() * (96 - 8) + 8;
		}
		RectHazard* testWall = new DevWallHazard(x_start + RNG::randFunc() * (area_width - width), y_start + RNG::randFunc() * (area_height - height), width, height);
		if (testWall->reasonableLocation()) {
			randomized = testWall;
			break;
		} else {
			delete testWall;
		}
		attempts++;
	} while (attempts < 64);

	return randomized;
}
