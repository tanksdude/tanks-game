#include "circularnobulletzone.h"
#include "gamemanager.h"
#include "renderer.h"
#include "constants.h"
#include <math.h>
#include "mylib.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "collisionhandler.h"
#include "rng.h"

VertexArray* CircularNoBulletZone::va;
VertexBuffer* CircularNoBulletZone::vb;
IndexBuffer* CircularNoBulletZone::ib;
bool CircularNoBulletZone::initialized_GPU = false;

CircularNoBulletZone::CircularNoBulletZone(double xpos, double ypos, double radius) {
	x = xpos;
	y = ypos;
	r = radius;
	gameID = GameManager::getNextID();
	teamID = HAZARD_TEAM;

	canAcceptPowers = false;

	modifiesTankCollision = true;
	modifiesBulletCollision = true;

	initializeGPU();
}

CircularNoBulletZone::~CircularNoBulletZone() {
	//uninitializeGPU();
}

bool CircularNoBulletZone::initializeGPU() {
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

	vb = VertexBuffer::MakeVertexBuffer(positions, (Circle::numOfSides+1)*2 * sizeof(float), RenderingHints::static_draw);
	VertexBufferLayout layout(2);
	va = VertexArray::MakeVertexArray(*vb, layout);

	ib = IndexBuffer::MakeIndexBuffer(indices, Circle::numOfSides*3);

	initialized_GPU = true;
	return true;
}

bool CircularNoBulletZone::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;

	initialized_GPU = false;
	return true;
}

CircleHazard* CircularNoBulletZone::factory(int argc, std::string* argv) {
	if (argc >= 3) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double r = std::stod(argv[2]);
		return new CircularNoBulletZone(x, y, r);
	}
	return new CircularNoBulletZone(0, 0, 0);
}

bool CircularNoBulletZone::reasonableLocation() const {
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

void CircularNoBulletZone::draw() const {
	draw(x, y);
}

void CircularNoBulletZone::draw(double xpos, double ypos) const {
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM = Renderer::GenerateMatrix(r, r, 0, xpos, ypos);

	//TODO: make drawUnder() a thing
	ColorValueHolder color = getColor();
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *ib, *shader);
}

void CircularNoBulletZone::poseDraw() const {
	//TODO?
	return;
}

CircleHazard* CircularNoBulletZone::randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv) {
	int attempts = 0;
	CircleHazard* randomized = nullptr;
	double xpos, ypos, radius;
	do {
		if (argc >= 1) {
			radius = std::stod(argv[0]);
		} else {
			radius = RNG::randFunc2() * (20 - 10) + 10; //TODO: where should these constants be?
		}
		xpos = RNG::randFunc2() * (area_width - 2*radius) + (x_start + radius);
		ypos = RNG::randFunc2() * (area_height - 2*radius) + (y_start + radius);
		CircleHazard* testCircularNoBulletZone = new CircularNoBulletZone(xpos, ypos, radius);
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
