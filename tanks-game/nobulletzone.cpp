#include "nobulletzone.h"
#include "gamemanager.h"
#include "renderer.h"
#include "constants.h"
#include <math.h>
#include "mylib.h"
#include "backgroundrect.h"
#include "colormixer.h"
#include "wallmanager.h"
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
	return true;
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
