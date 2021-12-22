#include "stationary-turret-hazard.h"
#include "game-manager.h"
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
#include "collision-handler.h"
#include "rng.h"

VertexArray* StationaryTurretHazard::va;
VertexBuffer* StationaryTurretHazard::vb;
IndexBuffer* StationaryTurretHazard::ib;
VertexArray* StationaryTurretHazard::cannon_va;
VertexBuffer* StationaryTurretHazard::cannon_vb;
bool StationaryTurretHazard::initialized_GPU = false;

std::unordered_map<std::string, float> StationaryTurretHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

StationaryTurretHazard::StationaryTurretHazard(double xpos, double ypos, double angle, bool noGPU) {
	x = xpos;
	y = ypos;
	this->direction = SimpleVector2D(angle, 0, true);
	r = TANK_RADIUS / 4;
	gameID = GameManager::getNextID();
	teamID = HAZARD_TEAM;

	tickCount = 0;
	tickCycle = 100; //100 is JS default (because of shooting speed) and 200 just looks weird
	currentState = 0;
	maxState = 3;
	stateMultiplier = new double[maxState]{2, 1, 2};
	stateColors = new ColorValueHolder[maxState]{ {0.5f, 0.5f, 0.5f}, {1.0f, 0x22/255.0, 0x11/255.0}, {0.0f, 0.5f, 1.0f} };

	canAcceptPowers = false;
}

StationaryTurretHazard::StationaryTurretHazard(double xpos, double ypos, double angle)
: StationaryTurretHazard(xpos, ypos, angle, true) {
	initializeGPU();
}

StationaryTurretHazard::StationaryTurretHazard(double xpos, double ypos, double angle, double radius) : StationaryTurretHazard(xpos, ypos, angle) {
	r = radius;
}

StationaryTurretHazard::~StationaryTurretHazard() {
	delete[] stateMultiplier;
	delete[] stateColors;

	//uninitializeGPU();
}

bool StationaryTurretHazard::initializeGPU() {
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

	vb = VertexBuffer::MakeVertexBuffer(positions, (Circle::numOfSides+1)*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout layout(2);
	va = VertexArray::MakeVertexArray(*vb, layout);

	ib = IndexBuffer::MakeIndexBuffer(indices, Circle::numOfSides*3);

	float cannon_positions[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	cannon_vb = VertexBuffer::MakeVertexBuffer(cannon_positions, 2*2 * sizeof(float));
	VertexBufferLayout cannon_layout(2);
	cannon_va = VertexArray::MakeVertexArray(*cannon_vb, cannon_layout);

	initialized_GPU = true;
	return true;
}

bool StationaryTurretHazard::uninitializeGPU() {
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

CircleHazard* StationaryTurretHazard::factory(int argc, std::string* argv) {
	if (argc >= 3) {
		double x = std::stod(argv[0]);
		double y = std::stod(argv[1]);
		double a = std::stod(argv[2]);
		if (argc >= 4) {
			double r = std::stod(argv[3]);
			return new StationaryTurretHazard(x, y, a, r);
		}
		return new StationaryTurretHazard(x, y, a);
	}
	return new StationaryTurretHazard(0, 0, 0);
}

/*
double StationaryTurretHazard::getAngle() const {
	return fmod(fmod(direction.getAngle(), 2*PI) + 2*PI, 2*PI);
}
*/

void StationaryTurretHazard::tick() {
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
		BulletManager::pushBullet(new Bullet(x + r*cos(direction.getAngle()), y + r*sin(direction.getAngle()), r*(BULLET_TO_TANK_RADIUS_RATIO*2), direction.getAngle(), Tank::default_maxSpeed*BULLET_TO_TANK_SPEED_RATIO, this->getTeamID(), BulletParentType::individual, this->getGameID()));
	}
}

bool StationaryTurretHazard::canSeeTank(const Tank* t) const {
	SimpleVector2D distToTank = SimpleVector2D(t->getX() - x, t->getY() - y);
	Circle* p = new Point(x + distToTank.getMagnitude()*cos(this->direction.getAngle()), y + distToTank.getMagnitude()*sin(this->direction.getAngle()));
	if (!CollisionHandler::fullyCollided(p, t)) {
		delete p;
		return false; //not pointing at tank (it's an approximation but it's good enough)
	}
	//check walls
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		if (CollisionHandler::lineRectCollision(x, y, p->x, p->y, wa)) {
			delete p;
			return false;
		}
	}
	delete p;
	return true;
}

bool StationaryTurretHazard::reasonableLocation() const {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		if (canSeeTank(TankManager::getTank(i))) {
			return false;
		}
	}

	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		if (CollisionHandler::partiallyCollided(this, WallManager::getWall(i))) {
			return false;
		}
	}

	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		CircleHazard* ch = HazardManager::getCircleHazard(i);
		if (ch->getGameID() != this->getGameID()) {
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

ColorValueHolder StationaryTurretHazard::getColor() const {
	return ColorMixer::mix(stateColors[currentState], stateColors[(currentState+1)%maxState], constrain<double>(tickCount/(tickCycle*stateMultiplier[currentState]), 0, 1));
}

ColorValueHolder StationaryTurretHazard::getColor(int state) const {
	if (state < 0) {
		return stateColors[0];
	}
	return stateColors[state % maxState];
}

void StationaryTurretHazard::draw() const {
	drawBody();
	drawOutline();
	drawBarrel();
}

void StationaryTurretHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for StationaryTurretHazard::draw!" << std::endl;
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

void StationaryTurretHazard::poseDraw() const {
	//TODO: adjust so drawBody will only draw with the normal color?
	drawBody();
	drawOutline();
	drawBarrel();
}

void StationaryTurretHazard::poseDraw(DrawingLayers layer) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for StationaryTurretHazard::poseDraw!" << std::endl;
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

void StationaryTurretHazard::ghostDraw(float alpha) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	drawBody(alpha);
	drawOutline(alpha);
	drawBarrel(alpha);
}

void StationaryTurretHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for StationaryTurretHazard::ghostDraw!" << std::endl;
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

inline void StationaryTurretHazard::drawBody(float alpha) const {
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

inline void StationaryTurretHazard::drawOutline(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	glLineWidth(1.0f);

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void StationaryTurretHazard::drawBarrel(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 MVPM;

	glLineWidth(2.0f);

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	MVPM = Renderer::GenerateMatrix(r, 1, direction.getAngle(), x, y);
	shader->setUniformMat4f("u_MVP", MVPM);

	Renderer::Draw(*cannon_va, *shader, GL_LINES, 0, 2);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/*
void StationaryTurretHazard::drawCPU() const {
	//main body:
	ColorValueHolder color = getColor();
	glColor3f(color.getRf(), color.getGf(), color.getBf());

	glBegin(GL_POLYGON);

	for (int i = 0; i < Circle::numOfSides; i++) {
		glVertex3f(x + r*cos(i * 2*PI / Circle::numOfSides), y + r*sin(i * 2*PI / Circle::numOfSides), 0);
	}

	glEnd();

	//outline:
	glColor3f(0, 0, 0);
	glLineWidth(2);

	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < Circle::numOfSides; i++) {
		glVertex2f(x + r*cos(i * 2*PI / Circle::numOfSides), y + r*sin(i * 2*PI / Circle::numOfSides));
	}

	glEnd();

	//barrel:
	glColor3f(0, 0, 0);
	glLineWidth(2);

	glBegin(GL_LINES);

	glVertex2f(x, y);
	glVertex2f(x + r*cos(direction.getAngle()), y + r*sin(direction.getAngle()));

	glEnd();
}
*/

CircleHazard* StationaryTurretHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv) {
	int attempts = 0;
	CircleHazard* randomized = nullptr;
	double xpos, ypos, angle;
	if (argc >= 1) {
		angle = std::stod(argv[0]);
	} else {
		angle = RNG::randFunc() * 2*PI;
	}
	do {
		xpos = RNG::randFunc2() * (area_width - 2*TANK_RADIUS/4) + (x_start + TANK_RADIUS/4);
		ypos = RNG::randFunc2() * (area_height - 2*TANK_RADIUS/4) + (y_start + TANK_RADIUS/4);
		CircleHazard* testStationaryTurret = new StationaryTurretHazard(xpos, ypos, angle);
		if (testStationaryTurret->reasonableLocation()) {
			randomized = testStationaryTurret;
			break;
		} else {
			delete testStationaryTurret;
		}
		attempts++;
	} while (attempts < 128);

	return randomized;
}
