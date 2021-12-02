#include "rectangularnobulletzone.h"
#include "gamemanager.h"
#include "renderer.h"
#include "constants.h"
#include <math.h>
#include "colormixer.h"
#include "backgroundrect.h"
#include "mylib.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "collisionhandler.h"
#include "rng.h"

VertexArray* RectangularNoBulletZone::va;
VertexBuffer* RectangularNoBulletZone::vb;
IndexBuffer* RectangularNoBulletZone::ib;
bool RectangularNoBulletZone::initialized_GPU = false;

std::unordered_map<std::string, float> RectangularNoBulletZone::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .25f });
	weights.insert({ "old", .5f });
	weights.insert({ "random-old", .25f });
	weights.insert({ "random", .125f });
	return weights;
}

RectangularNoBulletZone::RectangularNoBulletZone(double xpos, double ypos, double width, double height) {
	x = xpos;
	y = ypos;
	w = width;
	h = height;
	gameID = GameManager::getNextID();
	teamID = HAZARD_TEAM;

	canAcceptPowers = false;

	modifiesTankCollision = true;
	modifiesBulletCollision = true;

	initializeGPU();
}

RectangularNoBulletZone::~RectangularNoBulletZone() {
	//uninitializeGPU();
}

bool RectangularNoBulletZone::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	//background:
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

	vb = VertexBuffer::MakeVertexBuffer(positions, 4*2 * sizeof(float), RenderingHints::static_draw);
	VertexBufferLayout layout(2);
	va = VertexArray::MakeVertexArray(*vb, layout);

	ib = IndexBuffer::MakeIndexBuffer(indices, 6);

	initialized_GPU = true;
	return true;
}

bool RectangularNoBulletZone::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;

	initialized_GPU = false;
	return true;
}

RectHazard* RectangularNoBulletZone::factory(int argc, std::string* argv) {
	if (argc >= 4) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double w = std::stod(argv[2]);
		double h = std::stod(argv[3]);
		return new RectangularNoBulletZone(x, y, w, h);
	}
	return new RectangularNoBulletZone(0, 0, 0, 0);
}

bool RectangularNoBulletZone::reasonableLocation() const {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		if (CollisionHandler::fullyCollided(this, WallManager::getWall(i))) {
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

void RectangularNoBulletZone::draw() const {
	ghostDraw(1.0f);
}

void RectangularNoBulletZone::draw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularNoBulletZone::draw!" << std::endl;
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

void RectangularNoBulletZone::poseDraw() const {
	draw();
}

void RectangularNoBulletZone::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularNoBulletZone::poseDraw!" << std::endl;
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

void RectangularNoBulletZone::ghostDraw(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	ColorValueHolder color = GeneralizedNoBulletZone::getColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(w, h, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *ib, *shader);
}

void RectangularNoBulletZone::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		default:
			std::cerr << "WARNING: unknown DrawingLayer for RectangularNoBulletZone::ghostDraw!" << std::endl;
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

RectHazard* RectangularNoBulletZone::randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv) {
	int attempts = 0;
	RectHazard* randomized = nullptr;
	double xpos, ypos, width, height;
	do {
		if (argc >= 2) {
			width = std::stod(argv[0]);
			height = std::stod(argv[1]);
		} else {
			width = RNG::randFunc2() * (40 - 20) + 20; //TODO: where should these constants be?
			height = RNG::randFunc2() * (50 - 20) + 20; //TODO: where should these constants be?
		}
		xpos = RNG::randFunc2() * (area_width - 2*width) + (x_start + width);
		ypos = RNG::randFunc2() * (area_height - 2*height) + (y_start + height);
		RectHazard* testRectangularNoBulletZone = new RectangularNoBulletZone(xpos, ypos, width, height);
		if (testRectangularNoBulletZone->reasonableLocation()) {
			randomized = testRectangularNoBulletZone;
			break;
		} else {
			delete testRectangularNoBulletZone;
		}
		attempts++;
	} while (attempts < 64);

	return randomized;
}
