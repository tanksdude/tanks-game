#include "targeting-turret-hazard.h"
#include "renderer.h"
#include "constants.h"
#include <cmath>
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

VertexArray* TargetingTurretHazard::va;
VertexBuffer* TargetingTurretHazard::vb;
IndexBuffer* TargetingTurretHazard::ib;
VertexArray* TargetingTurretHazard::cannon_va;
VertexBuffer* TargetingTurretHazard::cannon_vb;
VertexArray* TargetingTurretHazard::reticule_va;
VertexBuffer* TargetingTurretHazard::reticule_vb;
bool TargetingTurretHazard::initialized_GPU = false;

std::unordered_map<std::string, float> TargetingTurretHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

TargetingTurretHazard::TargetingTurretHazard(double xpos, double ypos, double angle, bool) : StationaryTurretHazard(xpos, ypos, angle, true) {
	//x = xpos;
	//y = ypos;
	//velocity = SimpleVector2D(angle, 0, true);
	r = TANK_RADIUS / 2;

	targeting = false;
	targetingX = this->x;
	targetingY = this->y;
	targetingCount = 0;
	trackingID = this->getGameID();
	ColorValueHolder temp[2] = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} };
	std::copy(temp, temp+2, reticuleColors);

	canAcceptPowers = false; //... true?
}

TargetingTurretHazard::TargetingTurretHazard(double xpos, double ypos, double angle) : TargetingTurretHazard(xpos, ypos, angle, true) {
	initializeGPU();
}

TargetingTurretHazard::TargetingTurretHazard(double xpos, double ypos, double angle, double radius) : TargetingTurretHazard(xpos, ypos, angle) {
	r = radius;
}

TargetingTurretHazard::~TargetingTurretHazard() {
	//delete[] stateMultiplier;
	//delete[] stateColors;

	//uninitializeGPU();
}

bool TargetingTurretHazard::initializeGPU() {
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
	VertexBufferLayout layout = {
		{ ShaderDataType::Float2, "a_Position" },
		//{ ShaderDataType::Float4, "a_Color" }
	};
	vb->SetLayout(layout);

	ib = IndexBuffer::MakeIndexBuffer(indices, Circle::numOfSides*3);

	va = VertexArray::MakeVertexArray();
	va->AddVertexBuffer(vb);
	va->SetIndexBuffer(ib);

	//cannon:
	float cannon_positions[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	cannon_vb = VertexBuffer::MakeVertexBuffer(cannon_positions, 2*2 * sizeof(float), RenderingHints::dynamic_draw);
	VertexBufferLayout cannon_layout = {
		{ ShaderDataType::Float2, "a_Position" },
		//{ ShaderDataType::Float4, "a_Color" }
	};
	cannon_vb->SetLayout(layout);

	cannon_va = VertexArray::MakeVertexArray();
	cannon_va->AddVertexBuffer(cannon_vb);

	//targeting reticule:
	//the circle is the same as the body
	float reticule_positions[16] = {
		 0.75f,  0.0f,   1.25f,  0.0f,  //right
		 0.0f,   0.75f,  0.0f,   1.25f, //up
		-0.75f,  0.0f,  -1.25f,  0.0f,  //left
		 0.0f,  -0.75f,  0.0f,  -1.25f  //down
	};
	reticule_vb = VertexBuffer::MakeVertexBuffer(reticule_positions, 16*2 * sizeof(float));
	VertexBufferLayout reticule_layout = {
		{ ShaderDataType::Float2, "a_Position" },
		//{ ShaderDataType::Float4, "a_Color" }
	};
	reticule_vb->SetLayout(layout);

	reticule_va = VertexArray::MakeVertexArray();
	reticule_va->AddVertexBuffer(reticule_vb);

	initialized_GPU = true;
	return true;
}

bool TargetingTurretHazard::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete va;
	delete vb;
	delete ib;
	delete cannon_va;
	delete cannon_vb;
	delete reticule_va;
	delete reticule_vb;

	initialized_GPU = false;
	return true;
}

CircleHazard* TargetingTurretHazard::factory(GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 3) {
			double* arr = (double*)(args.getDataPortion(0));
			double x = arr[0];
			double y = arr[1];
			double a = arr[2];
			if (count >= 4) {
				double r = arr[3];
				return new TargetingTurretHazard(x, y, a, r);
			}
			return new TargetingTurretHazard(x, y, a);
		}
	}
	return new TargetingTurretHazard(0, 0, 0);
}

inline void TargetingTurretHazard::updateTrackingPos(const Tank* t, bool pointedAt) {
	if (pointedAt) {
		targetingX = t->x;
		targetingY = t->y;
	} else {
		double dist = sqrt(pow(t->x - x, 2) + pow(t->y - y, 2));
		targetingX = x + dist * cos(velocity.getAngle());
		targetingY = y + dist * sin(velocity.getAngle());
	}
}

void TargetingTurretHazard::tick() {
	//tickCount is unused... maybe targetingCount should replace it...

	if (currentState == 0) { //either tracking a tank or doing nothing
		if (targeting) { //tracking tank
			tick_continueTracking();
		}
		if (!targeting) { //looking for a tank
			tick_lookForNewTarget();
		}
	}

	if (currentState == 1) { //charging up to shoot (and shooting)
		tick_chargeUp();
	}

	if (currentState == 2) { //cooling down
		tick_cooldown();
	}

	//maxState is 3; not using else in case tickCycle is zero
}

inline void TargetingTurretHazard::tick_continueTracking() {
	bool tankIsVisible = false;
	const Tank* t = TankManager::getTankByID(this->trackingID);
	if (t != nullptr) { //exists
		if (canSeeTank(t)) { //no obstructions
			tankIsVisible = true;
			//main logic here
			turnTowardsTank(t);
			if (isPointedAt(t)) {
				targetingCount++;
				if (targetingCount >= stateMultiplier[0] * tickCycle) {
					currentState = 1;
					targetingCount = 0;
				}
				updateTrackingPos(t, true);
			} else {
				targetingCount--;
				if (targetingCount < 0) {
					targetingCount = 0;
				}
				updateTrackingPos(t, false);
			}
		}
	}
	if (!tankIsVisible) {
		targeting = false;
	}
}

inline void TargetingTurretHazard::tick_lookForNewTarget() {
	targetingCount = 0;

	std::vector<bool> tankVisibility; tankVisibility.reserve(TankManager::getNumTanks()); //not using regular arrays so people (including future me) can actually read this
	std::vector<double> distancesToTank; distancesToTank.reserve(TankManager::getNumTanks()); //TODO: option for angle-based selection (look at homing in PowerFunctionHelper)
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		Tank* t = TankManager::getTank(i);
		tankVisibility.push_back(canSeeTank(t));
		if (tankVisibility.at(i)) {
			distancesToTank.push_back(sqrt(pow(x - t->x, 2) + pow(y - t->y, 2)));
		} else {
			distancesToTank.push_back(GAME_WIDTH*2 + GAME_HEIGHT*2);
		}
	}

	double minDist = GAME_WIDTH*2 + GAME_HEIGHT*2;
	std::vector<int> tankIndices; tankIndices.reserve(TankManager::getNumTanks()); //multiple tanks can have same distance
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		if (distancesToTank[i] == minDist) {
			tankIndices.push_back(i);
		} else if (distancesToTank[i] < minDist) {
			tankIndices.clear();
			tankIndices.push_back(i);
			minDist = distancesToTank[i];
		}
	}

	int indexOfTargetedTank;
	if (tankIndices.size() == 1) {
		indexOfTargetedTank = tankIndices[0];
	} else {
		indexOfTargetedTank = tankIndices[int(RNG::randFunc() * tankIndices.size())];
	}

	if (tankVisibility[indexOfTargetedTank]) {
		const Tank* t = TankManager::getTank(indexOfTargetedTank);
		trackingID = t->getGameID();
		targeting = true;
		updateTrackingPos(t, isPointedAt(t));
	}
}

inline void TargetingTurretHazard::tick_chargeUp() {
	targetingCount++;
	if (targetingCount >= stateMultiplier[1] * tickCycle) {
		BulletManager::pushBullet(new Bullet(x + r*cos(velocity.getAngle()), y + r*sin(velocity.getAngle()), r*BULLET_TO_TANK_RADIUS_RATIO, velocity.getAngle(), Tank::default_maxSpeed*BULLET_TO_TANK_SPEED_RATIO, this->getTeamID(), BulletParentType::individual, this->getGameID()));
		currentState = 2;
		targetingCount = 0;
		targeting = false; //allows target to change (also controls whether the reticule is drawn)
	}
}

inline void TargetingTurretHazard::tick_cooldown() {
	targetingCount++;
	if (targetingCount >= stateMultiplier[2] * tickCycle) {
		targetingCount = 0;
		currentState = 0;
	}
}

bool TargetingTurretHazard::canSeeTank(const Tank* t) const {
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		Wall* wa = WallManager::getWall(i);
		if (CollisionHandler::lineRectCollision(x, y, t->x, t->y, wa)) {
			return false;
		}
	}
	return true;
}

void TargetingTurretHazard::turnTowardsTank(const Tank* t) {
	//see PowerFunctionHelper::homingGeneric
	SimpleVector2D distToTank = SimpleVector2D(t->getX() - this->x, t->getY() - this->y);
	float theta = SimpleVector2D::angleBetween(distToTank, velocity);
	if (abs(theta) < PI/turningIncrement) {
		//too small to adjust angle
	} else {
		//large angle adjustment needed
		if (theta < 0) {
			this->velocity.changeAngle(PI/turningIncrement);
		} else {
			this->velocity.changeAngle(-PI/turningIncrement);
		}
	}
}

bool TargetingTurretHazard::isPointedAt(const Tank* t) const {
	return (abs(SimpleVector2D::angleBetween(velocity, SimpleVector2D(t->x - x, t->y - y))) < PI/turningIncrement);
}

bool TargetingTurretHazard::reasonableLocation() const {
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

ColorValueHolder TargetingTurretHazard::getColor() const {
	return ColorMixer::mix(stateColors[currentState], stateColors[(currentState+1)%maxState], constrain<double>(targetingCount/(tickCycle*stateMultiplier[currentState]), 0, 1));
}

ColorValueHolder TargetingTurretHazard::getColor(int state) const {
	if (state < 0) {
		return stateColors[0];
	}
	return stateColors[state % maxState];
}

ColorValueHolder TargetingTurretHazard::getReticuleColor() const {
	if (currentState == 0) {
		return reticuleColors[0];
	}
	if (currentState == 1) {
		return reticuleColors[1];
	}
	return ColorValueHolder(0.0f, 0.0f, 0.0f); //shouldn't be drawn
}

void TargetingTurretHazard::draw() const {
	drawBody();
	drawOutline();
	drawBarrel();
	drawReticule();
}

void TargetingTurretHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for TargetingTurretHazard::draw!" << std::endl;
		case DrawingLayers::normal:
			drawBody();
			drawOutline();
			drawBarrel();
			break;

		case DrawingLayers::effects:
			drawReticule();
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void TargetingTurretHazard::poseDraw() const {
	//TODO: adjust so drawBody will only draw with the normal color?
	drawBody();
	drawOutline();
	drawBarrel();
}

void TargetingTurretHazard::poseDraw(DrawingLayers layer) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for TargetingTurretHazard::poseDraw!" << std::endl;
		case DrawingLayers::normal:
			drawBody();
			drawOutline();
			drawBarrel();
			break;

		case DrawingLayers::effects:
			//drawReticule();
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void TargetingTurretHazard::ghostDraw(float alpha) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	drawBody(alpha);
	drawOutline(alpha);
	drawBarrel(alpha);
}

void TargetingTurretHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for TargetingTurretHazard::ghostDraw!" << std::endl;
		case DrawingLayers::normal:
			drawBody(alpha);
			drawOutline(alpha);
			drawBarrel(alpha);
			break;

		case DrawingLayers::effects:
			//drawReticule(alpha);
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

inline void TargetingTurretHazard::drawBody(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	ColorValueHolder color = getColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*va, *ib, *shader);
}

inline void TargetingTurretHazard::drawOutline(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	glLineWidth(1.0f);

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r, r, 0, x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void TargetingTurretHazard::drawBarrel(float alpha) const {
	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	glLineWidth(2.0f);

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	modelMatrix = Renderer::GenerateModelMatrix(r, 1, velocity.getAngle(), x, y);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*cannon_va, *shader, GL_LINES, 0, 2);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

inline void TargetingTurretHazard::drawReticule(float alpha) const {
	if (!targeting) {
		return;
	}

	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	glLineWidth(2.0f);

	ColorValueHolder color = getReticuleColor();
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	/*
	if (currentState == 0) {
		modelMatrix = Renderer::GenerateMatrix(2*TANK_RADIUS, 2*TANK_RADIUS, -PI/2 * targetingCount/(stateMultiplier[0] * tickCycle), targetingX, targetingY);
	} else {
		modelMatrix = Renderer::GenerateMatrix(2*TANK_RADIUS, 2*TANK_RADIUS, -PI/2, targetingX, targetingY);
	}
	*/
	modelMatrix = Renderer::GenerateModelMatrix(2*TANK_RADIUS, 2*TANK_RADIUS, 0, targetingX, targetingY);
	shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

	Renderer::Draw(*va, *shader, GL_LINE_LOOP, 1, Circle::numOfSides);
	Renderer::Draw(*reticule_va, *shader, GL_LINES, 0, 8);

	//cleanup
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/*
void TargetingTurretHazard::drawCPU() const {
	//nah
}
*/

CircleHazard* TargetingTurretHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, GenericFactoryConstructionData& args) {
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
		xpos = RNG::randFunc() * (area_width - 2*TANK_RADIUS/2) + (x_start + TANK_RADIUS/2);
		ypos = RNG::randFunc() * (area_height - 2*TANK_RADIUS/2) + (y_start + TANK_RADIUS/2);
		CircleHazard* testTargetingTurret = new TargetingTurretHazard(xpos, ypos, angle);
		if (testTargetingTurret->reasonableLocation()) {
			randomized = testTargetingTurret;
			break;
		} else {
			delete testTargetingTurret;
		}
		attempts++;
	} while (attempts < 128);

	return randomized;
}
