#include "patrolling-turret-hazard.h"
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

VertexArray* PatrollingTurretHazard::va;
VertexBuffer* PatrollingTurretHazard::vb;
IndexBuffer* PatrollingTurretHazard::ib;
VertexArray* PatrollingTurretHazard::cannon_va;
VertexBuffer* PatrollingTurretHazard::cannon_vb;
VertexArray* PatrollingTurretHazard::reticule_va;
VertexBuffer* PatrollingTurretHazard::reticule_vb;
bool PatrollingTurretHazard::initialized_GPU = false;

std::unordered_map<std::string, float> PatrollingTurretHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

PatrollingTurretHazard::PatrollingTurretHazard(double xpos, double ypos, double angle, int pairNum, double* posList, double* waitList) : TargetingTurretHazard(xpos, ypos, angle, true) {
	//x = xpos;
	//y = ypos;
	velocity = SimpleVector2D(angle, Tank::default_maxSpeed/2, true);
	//r = TANK_RADIUS / 2;

	//targeting = false;
	//targetingX = this->x;
	//targetingY = this->y;
	//targetingCount = 0;
	//trackingID = this->getGameID();
	//ColorValueHolder temp[2] = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} };
	//std::copy(temp, temp+2, reticuleColors);

	currentPatrolTarget = 0;
	waitCount = 0;

	routePosPairNum = pairNum;
	if (posList == nullptr) {
		routePosList = nullptr;
		routeWaitCount = nullptr;
	} else {
		routePosList = new double[pairNum*2];
		//for (int i = 0; i < pairNum*2; i++) {
		//	routePosList[i] = posList[i];
		//}
		std::copy(posList, posList+(pairNum*2), routePosList);

		routeWaitCount = new double[pairNum];
		if (waitList == nullptr) {
			for (int i = 0; i < pairNum; i++) {
				routeWaitCount[i] = 200;
			}
		} else {
			std::copy(waitList, waitList+(pairNum), routeWaitCount);
		}
	}

	canAcceptPowers = true; //TODO

	initializeGPU();
}

PatrollingTurretHazard::~PatrollingTurretHazard() {
	if (routePosList != nullptr) {
		delete[] routePosList;
	}
	if (routeWaitCount != nullptr) {
		delete[] routeWaitCount;
	}

	//uninitializeGPU();
}

bool PatrollingTurretHazard::initializeGPU() {
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

	//targeting reticule:
	//the circle is the same as the body
	float reticule_positions[16] = {
		 0.75f,  0.0f,   1.25f,  0.0f,  //right
		 0.0f,   0.75f,  0.0f,   1.25f, //up
		-0.75f,  0.0f,  -1.25f,  0.0f,  //left
		 0.0f,  -0.75f,  0.0f,  -1.25f  //down
	};
	reticule_vb = VertexBuffer::MakeVertexBuffer(reticule_positions, 16*2 * sizeof(float));
	VertexBufferLayout reticule_layout(2);
	reticule_va = VertexArray::MakeVertexArray(*reticule_vb, reticule_layout);

	initialized_GPU = true;
	return true;
}

bool PatrollingTurretHazard::uninitializeGPU() {
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

CircleHazard* PatrollingTurretHazard::factory(GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 3) {
		int count_basic = args.getDataPortionLength(0);
		int count_num = args.getDataPortionLength(1);
		int count_list = args.getDataPortionLength(2);

		double x, y, a;
		int num;
		double* list;

		if (count_basic >= 3) {
			double* arr_basic = (double*)args.getDataPortion(0);
			x = arr_basic[0];
			y = arr_basic[1];
			a = arr_basic[2];

			if ((count_num >= 1) && (count_list >= 1)) {
				int* arr_num = (int*)args.getDataPortion(1);
				double* arr_list = (double*)args.getDataPortion(2);
				num = arr_num[0];
				list = arr_list;

				if ((args.getDataCount() >= 4) && (args.getDataPortionLength(3) >= 1)) {
					double* wait = (double*)args.getDataPortion(3);
					return new PatrollingTurretHazard(x, y, a, num, list, wait);
				} else {
					return new PatrollingTurretHazard(x, y, a, num, list, nullptr);
				}
			} else {
				return new PatrollingTurretHazard(x, y, a, 0, nullptr, nullptr);
			}
		}
	}

	return new PatrollingTurretHazard(0, 0, 0, 0, nullptr, nullptr);
}

inline double PatrollingTurretHazard::getRoutePosX(int i) const {
	return routePosList[i*2];
}
inline double PatrollingTurretHazard::getRoutePosY(int i) const {
	return routePosList[i*2+1];
}

void PatrollingTurretHazard::tick() {
	TargetingTurretHazard::tick();
	//overrides tick_lookForNewTarget to do patrolling and stuff
}

inline void PatrollingTurretHazard::tick_lookForNewTarget() {
	TargetingTurretHazard::tick_lookForNewTarget();

	if (targeting) {
		return;
	}

	tick_patrol();
}

inline void PatrollingTurretHazard::tick_patrol() {
	if (isWaitingAtPoint()) {
		tick_patrolWait();
	} else {
		turnTowardsPoint(/* TODO */);
		if (isPointedAtPoint(/* TODO */)) {
			tick_moveForward();
		}
	}
}

inline void PatrollingTurretHazard::tick_moveForward() {
	this->x += velocity.getXComp();
	this->y += velocity.getYComp();
}

inline void PatrollingTurretHazard::tick_patrolWait() {
	if (waitCount >= routeWaitCount[currentPatrolTarget]) {
		waitCount = 0;
		currentPatrolTarget = (currentPatrolTarget+1) % routePosPairNum;
	} else {
		waitCount++;
	}
}

bool PatrollingTurretHazard::isCloseAsPossibleToCurrentPoint() const {
	//find distance from current position to target point
	//if distance is less than current velocity, not close enough
	double dist = sqrt(pow(this->x - getRoutePosX(currentPatrolTarget), 2) + pow(this->y - getRoutePosY(currentPatrolTarget), 2));
	return (dist < velocity.getMagnitude());
}

bool PatrollingTurretHazard::isWaitingAtPoint() const {
	return isCloseAsPossibleToCurrentPoint();
}

void PatrollingTurretHazard::turnTowardsPoint() {
	//see TargetingTurretHazard::turnTowardsTank
	SimpleVector2D distToPoint = SimpleVector2D(getRoutePosX(currentPatrolTarget) - this->x, getRoutePosY(currentPatrolTarget) - this->y);
	float theta = SimpleVector2D::angleBetween(distToPoint, velocity);
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

bool PatrollingTurretHazard::isPointedAtPoint() const {
	SimpleVector2D distToPoint = SimpleVector2D(getRoutePosX(currentPatrolTarget) - this->x, getRoutePosY(currentPatrolTarget) - this->y);
	float theta = SimpleVector2D::angleBetween(distToPoint, velocity);
	return (abs(theta) < PI/turningIncrement);
}

bool PatrollingTurretHazard::reasonableLocation() const {
	//this is only an approximation but there should only be problems for ultra-tiny walls
	//for each segment of the path, generate an "inner" and "outer" path, r distance away
	//check the inner, outer, and normal paths against every wall and hazard (ignore other patrolling turrets for now)
	//might want to check if any point along the path can see the tank, but that would need triangle-rectangle collision detection, so... not doing that for now

	if (routePosPairNum <= 1) {
		return TargetingTurretHazard::reasonableLocation();
	}

	for (int i = 0; i < routePosPairNum; i++) {
		const int start_pos = i;
		const int end_pos = (i+1) % routePosPairNum;

		SimpleVector2D path = SimpleVector2D(getRoutePosX(end_pos) - getRoutePosX(start_pos), getRoutePosY(end_pos), getRoutePosY(start_pos));
		double outerPath[4] = {
			getRoutePosX(start_pos) + getR() * cos(path.getAngle() - PI/2), getRoutePosY(start_pos) + getR() * sin(path.getAngle() - PI/2),
			getRoutePosX(end_pos)   + getR() * cos(path.getAngle() - PI/2), getRoutePosY(end_pos)   + getR() * sin(path.getAngle() - PI/2) };
		double mainPath[4] = {
			getRoutePosX(start_pos), getRoutePosY(start_pos),
			getRoutePosX(end_pos), getRoutePosY(end_pos) };
		double innerPath[4] = {
			getRoutePosX(start_pos) + getR() * cos(path.getAngle() - PI/2), getRoutePosY(start_pos) + getR() * sin(path.getAngle() - PI/2),
			getRoutePosX(end_pos)   + getR() * cos(path.getAngle() - PI/2), getRoutePosY(end_pos)   + getR() * sin(path.getAngle() - PI/2) };

		/*
		for (int i = 0; i < TankManager::getNumTanks(); i++) {
			if (canSeeTank(TankManager::getTank(i))) {
				return false;
			}
		}
		*/

		for (int i = 0; i < WallManager::getNumWalls(); i++) {
			if (CollisionHandler::lineRectCollision(outerPath[0], outerPath[1], outerPath[2], outerPath[3], WallManager::getWall(i))) {
				return false;
			}
			if (CollisionHandler::lineRectCollision(mainPath[0], mainPath[1], mainPath[2], mainPath[3], WallManager::getWall(i))) {
				return false;
			}
			if (CollisionHandler::lineRectCollision(innerPath[0], innerPath[1], innerPath[2], innerPath[3], WallManager::getWall(i))) {
				return false;
			}
		}

		//TODO
		/*
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
		*/
	}

	return TargetingTurretHazard::reasonableLocation();
}

void PatrollingTurretHazard::draw() const {
	drawBody();
	drawOutline();
	drawBarrel();
	drawReticule();
}

void PatrollingTurretHazard::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for PatrollingTurretHazard::draw!" << std::endl;
		case DrawingLayers::normal:
			drawBody();
			drawOutline();
			drawBarrel();
			break;

		case DrawingLayers::effects:
			drawReticule();
			//drawPath();
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			drawPath();
			break;
	}
}

void PatrollingTurretHazard::poseDraw() const {
	//TODO: adjust so drawBody will only draw with the normal color?
	drawBody();
	drawOutline();
	drawBarrel();
}

void PatrollingTurretHazard::poseDraw(DrawingLayers layer) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for PatrollingTurretHazard::poseDraw!" << std::endl;
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
			drawPath();
			break;
	}
}

void PatrollingTurretHazard::ghostDraw(float alpha) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	drawBody(alpha);
	drawOutline(alpha);
	drawBarrel(alpha);
}

void PatrollingTurretHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	//TODO: adjust so drawBody will only draw with the normal color?
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for PatrollingTurretHazard::ghostDraw!" << std::endl;
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

inline void PatrollingTurretHazard::drawBody(float alpha) const {
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

inline void PatrollingTurretHazard::drawOutline(float alpha) const {
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

inline void PatrollingTurretHazard::drawBarrel(float alpha) const {
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

inline void PatrollingTurretHazard::drawReticule(float alpha) const {
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

inline void PatrollingTurretHazard::drawPath(float alpha) const {
	//TODO: this is temporary; the path should have its own VA/VB/IB (not stealing the barrel's stuff)

	alpha = constrain<float>(alpha, 0, 1);
	alpha = alpha * alpha;
	Shader* shader = Renderer::getShader("main");
	glm::mat4 modelMatrix;

	//circles
	ColorValueHolder color = ColorValueHolder(0, 0, 0);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	shader->setUniform4f("u_color", color.getRf(), color.getGf(), color.getBf(), color.getAf());

	for (int i = 0; i < routePosPairNum; i++) {
		modelMatrix = Renderer::GenerateModelMatrix(r/2, r/2, 0, getRoutePosX(i), getRoutePosY(i));
		shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

		Renderer::Draw(*va, *ib, *shader);
	}

	//lines
	for (int i = 0; i < routePosPairNum; i++) {
		SimpleVector2D dist = SimpleVector2D(getRoutePosX((i+1) % routePosPairNum) - getRoutePosX(i), getRoutePosY((i+1) % routePosPairNum) - getRoutePosY(i));
		modelMatrix = Renderer::GenerateModelMatrix(dist.getMagnitude(), 1, dist.getAngle(), getRoutePosX(i), getRoutePosY(i));
		shader->setUniformMat4f("u_ModelMatrix", modelMatrix);

		Renderer::Draw(*cannon_va, *shader, GL_LINES, 0, 2);
	}
}

CircleHazard* PatrollingTurretHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, GenericFactoryConstructionData& args) {
	//TODO
	return nullptr;

	/*
	int attempts = 0;
	CircleHazard* randomized = nullptr;
	double xpos, ypos, angle;
	//if (argc >= 1) {
	//	angle = std::stod(argv[0]);
	//} else {
		angle = RNG::randFunc() * 2*PI;
	//}
	do {
		xpos = RNG::randFunc() * (area_width - 2*TANK_RADIUS/2) + (x_start + TANK_RADIUS/2);
		ypos = RNG::randFunc() * (area_height - 2*TANK_RADIUS/2) + (y_start + TANK_RADIUS/2);
		CircleHazard* testTargetingTurret = new PatrollingTurretHazard(xpos, ypos, angle);
		if (testTargetingTurret->reasonableLocation()) {
			randomized = testTargetingTurret;
			break;
		} else {
			delete testTargetingTurret;
		}
		attempts++;
	} while (attempts < 128);

	return randomized;
	*/
}
