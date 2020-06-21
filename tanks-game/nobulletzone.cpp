#include "nobulletzone.h"
#include "gamemanager.h"
#include "renderer.h"
#include "constants.h"
#include <math.h>
#include "mylib.h"
#include "backgroundrect.h"
#include "colormixer.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "collisionhandler.h"

VertexArray* NoBulletZone::va;
VertexBuffer* NoBulletZone::vb;
IndexBuffer* NoBulletZone::ib;
bool NoBulletZone::initialized_GPU = false;

NoBulletZone::NoBulletZone(double xpos, double ypos, double width, double height) {
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

NoBulletZone::~NoBulletZone() {
	//uninitializeGPU();
}

bool NoBulletZone::initializeGPU() {
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

	vb = new VertexBuffer(positions, 4*2 * sizeof(float), GL_DYNAMIC_DRAW);
	VertexBufferLayout layout(2);
	va = new VertexArray(*vb, layout);

	ib = new IndexBuffer(indices, 6);

	initialized_GPU = true;
	return true;
}

bool NoBulletZone::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;

	initialized_GPU = false;
	return true;
}

RectHazard* NoBulletZone::factory(int argc, std::string* argv) {
	if (argc >= 4) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double w = std::stod(argv[2]);
		double h = std::stod(argv[3]);
		return new NoBulletZone(x, y, w, h);
	}
	return new NoBulletZone(0, 0, 0, 0);
}

void NoBulletZone::tick() {
	return;
}

ColorValueHolder NoBulletZone::getColor() {
	return ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(.5f, .5f, .5f), .125);
}

bool NoBulletZone::reasonableLocation() {
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

void NoBulletZone::draw() {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(w, h, 0, x, y);
	
	//TODO: make drawUnder() a thing
	ColorValueHolder color = getColor();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *ib, *shader);
}

void NoBulletZone::drawCPU() {
	
}

RectHazard* NoBulletZone::randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv) {
	int attempts = 0;
	RectHazard* randomized = nullptr;
	double xpos, ypos, width, height;
	do {
		if (argc >= 2) {
			width = std::stod(argv[0]);
			height = std::stod(argv[1]);
		} else {
			width = randFunc2() * (40 - 20) + 20; //TODO: where should these constants be?
			height = randFunc2() * (50 - 20) + 20; //TODO: where should these constants be?
		}
		xpos = randFunc2() * (area_width - 2*width) + (x_start + width);
		ypos = randFunc2() * (area_height - 2*height) + (y_start + height);
		RectHazard* testNoBulletZone = new NoBulletZone(xpos, ypos, width, height);
		if (testNoBulletZone->reasonableLocation()) {
			randomized = testNoBulletZone;
			break;
		} else {
			delete testNoBulletZone;
		}
		attempts++;
	} while (attempts < 64);

	return randomized;
}
