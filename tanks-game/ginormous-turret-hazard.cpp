#include "ginormous-turret-hazard.h"
#include "game-manager.h"
#include "renderer.h"
#include "constants.h"
#include <math.h>
#include "color-mixer.h"
#include "background-rect.h"
#include <algorithm>
#include "mylib.h"
#include "tank.h"
#include "tank-manager.h"
#include "bullet-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "collision-handler.h"
#include "rng.h"
#include <iostream>

VertexArray* GinormousTurretHazard::va;
VertexBuffer* GinormousTurretHazard::vb;
IndexBuffer* GinormousTurretHazard::ib;
VertexArray* GinormousTurretHazard::cannon_va;
VertexBuffer* GinormousTurretHazard::cannon_vb;
bool GinormousTurretHazard::initialized_GPU = false;

std::unordered_map<std::string, float> GinormousTurretHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.25f });
	weights.insert({ "random-dev", 0.125f });
	return weights;
}

GinormousTurretHazard::GinormousTurretHazard(double xpos, double ypos, double angle) : TargetingTurretHazard(xpos, ypos, angle, true) {
	//x = xpos;
	//y = ypos;
	//velocity = SimpleVector2D(angle, 0, true);
	r = TANK_RADIUS * 4; // MotherTurretHazard radius * 2 //... should it be bigger?
	//gameID = GameManager::getNextID();
	//teamID = HAZARD_TEAM;

	for (int i = 0; i < maxState; i++) {
		stateMultiplier[i] *= 2;
	}
	turningIncrement *= 4;

	bulletCount = 8 * 8;

	canAcceptPowers = false;

	initializeGPU();
}

GinormousTurretHazard::~GinormousTurretHazard() {
	//uninitializeGPU();
}

bool GinormousTurretHazard::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	//body:
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

	vb = VertexBuffer::MakeVertexBuffer(positions, (Circle::numOfSides+1)*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout layout(2);
	va = VertexArray::MakeVertexArray(*vb, layout);

	ib = IndexBuffer::MakeIndexBuffer(indices, Circle::numOfSides*3);

	//cannon:
	float cannon_positions[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	cannon_vb = VertexBuffer::MakeVertexBuffer(cannon_positions, 2*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout cannon_layout(2);
	cannon_va = VertexArray::MakeVertexArray(*cannon_vb, cannon_layout);

	initialized_GPU = true;
	return true;
}

bool GinormousTurretHazard::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;
	delete cannon_va;
	delete cannon_vb;

	initialized_GPU = false;
	return true;
}

CircleHazard* GinormousTurretHazard::factory(GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 3) {
			double* arr = (double*)(args.getDataPortion(0));
			double x = arr[0];
			double y = arr[1];
			double a = arr[2];
			return new GinormousTurretHazard(x, y, a);
		}
	}
	return new GinormousTurretHazard(0, 0, 0);
}

void GinormousTurretHazard::tick() {
	//basically StationaryTurretHazard::tick

	tickCount++;
	bool mustShoot = false; //in case two state cycles happen at once (this will have annoying unit tests)
	while (tickCount >= tickCycle * stateMultiplier[currentState]) {
		if (tickCycle * stateMultiplier[currentState] <= 0) {
			tickCount = 0;
			currentState = 2;
			mustShoot = true;
			break;
		} else {
			tickCount -= tickCycle * stateMultiplier[currentState];
			++currentState %= maxState;
			if (currentState == 2) {
				mustShoot = true;
			}
		}
	}

	if (mustShoot) {
		const double angleDiff = 2*PI / bulletCount;
		for (int i = bulletCount; i >= 0; i--) {
			BulletManager::pushBullet(new Bullet(x + r*cos(i*angleDiff + velocity.getAngle()), y + r*sin(i*angleDiff + velocity.getAngle()), r*(BULLET_TO_TANK_RADIUS_RATIO*2), i*angleDiff + velocity.getAngle(), Tank::default_maxSpeed*BULLET_TO_TANK_SPEED_RATIO / 8, this->getTeamID(), BulletParentType::individual, this->getGameID()));
		}
	}
}

bool GinormousTurretHazard::reasonableLocation() const {
	return TargetingTurretHazard::reasonableLocation();
}

ColorValueHolder GinormousTurretHazard::getColor() const {
	return StationaryTurretHazard::getColor();
}

ColorValueHolder GinormousTurretHazard::getColor(int state) const {
	return StationaryTurretHazard::getColor(state);
}

void GinormousTurretHazard::draw() const {
	drawBody();
	drawOutline();
	drawBarrel();
}

void GinormousTurretHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for GinormousTurretHazard::draw!" << std::endl;
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

void GinormousTurretHazard::poseDraw() const {
	//TODO: adjust so drawBody will only draw with the normal color?
	drawBody();
	drawOutline();
	drawBarrel();
}

void GinormousTurretHazard::poseDraw(DrawingLayers layer) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for GinormousTurretHazard::poseDraw!" << std::endl;
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

void GinormousTurretHazard::ghostDraw(float alpha) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	drawBody(alpha);
	drawOutline(alpha);
	drawBarrel(alpha);
}

void GinormousTurretHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for GinormousTurretHazard::ghostDraw!" << std::endl;
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

inline void GinormousTurretHazard::drawBody(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	ColorValueHolder color = getColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *ib, *shader);
}

inline void GinormousTurretHazard::drawOutline(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	Renderer::SetLineWidth(1.0f);

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void GinormousTurretHazard::drawBarrel(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	Renderer::SetLineWidth(2.0f);

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(r, 1, velocity.getAngle(), x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*cannon_va, *shader, GL_LINES, 0, 2);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

CircleHazard* GinormousTurretHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, GenericFactoryConstructionData& args) {
	int attempts = 0;
	CircleHazard* randomized = nullptr;
	double xpos, ypos, angle;

	int count = 0;
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);
	}
	if (count >= 1) {
		double* arr = (double*)(args.getDataPortion(0));
		angle = arr[0];
	} else {
		angle = RNG::randFunc() * 2*PI;
	}

	do {
		xpos = RNG::randFunc2() * (area_width - 2*TANK_RADIUS/4) + (x_start + TANK_RADIUS/4);
		ypos = RNG::randFunc2() * (area_height - 2*TANK_RADIUS/4) + (y_start + TANK_RADIUS/4);
		CircleHazard* testGinormousTurret = new GinormousTurretHazard(xpos, ypos, angle);
		if (testGinormousTurret->reasonableLocation()) {
			randomized = testGinormousTurret;
			break;
		} else {
			delete testGinormousTurret;
		}
		attempts++;
	} while (attempts < 128);

	return randomized;
}
