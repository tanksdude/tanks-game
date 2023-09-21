#include "patrolling-turret-hazard.h"

#include "../constants.h"
#include <cmath>
#include <algorithm> //std::copy, std::clamp
#include <iostream>
#include "../rng.h"

#include "../renderer.h"
#include "../color-mixer.h"
#include "../background-rect.h"

#include "../collision-handler.h"
#include "../tank.h"
#include "../tank-manager.h"
#include "../bullet-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"

std::unordered_map<std::string, float> PatrollingTurretHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

PatrollingTurretHazard::PatrollingTurretHazard(double xpos, double ypos, double angle, int pairNum, const double* posList, const double* waitList) : TargetingTurretHazard(xpos, ypos, angle) {
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

	//canAcceptPowers = true; //TODO
}

PatrollingTurretHazard::~PatrollingTurretHazard() {
	if (routePosList != nullptr) {
		delete[] routePosList;
	}
	if (routeWaitCount != nullptr) {
		delete[] routeWaitCount;
	}
}

CircleHazard* PatrollingTurretHazard::factory(const GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 3) {
		int count_basic = args.getDataPortionLength(0);
		int count_num = args.getDataPortionLength(1);
		int count_list = args.getDataPortionLength(2);

		if (count_basic >= 3) {
			const double* arr_basic = static_cast<const double*>(args.getDataPortion(0).get());
			double x = arr_basic[0];
			double y = arr_basic[1];
			double a = arr_basic[2];

			if ((count_num >= 1) && (count_list >= 1)) {
				const int* arr_count = static_cast<const int*>(args.getDataPortion(1).get());
				const double* arr_list = static_cast<const double*>(args.getDataPortion(2).get());
				int count = arr_count[0];

				if ((args.getDataCount() >= 4) && (args.getDataPortionLength(3) >= 1)) {
					const double* wait = static_cast<const double*>(args.getDataPortion(3).get());
					return new PatrollingTurretHazard(x, y, a, count, arr_list, wait);
				} else {
					return new PatrollingTurretHazard(x, y, a, count, arr_list, nullptr);
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
	targetingCount = 0;
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
			std::cerr << "WARNING: unknown DrawingLayer for " + getName() + " draw!" << std::endl;
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
			if (Renderer::isDebugDrawingEnabled(this->getName())) {
				drawPath();
			}
			break;
	}
}

void PatrollingTurretHazard::poseDraw() const {
	drawBody();
	drawOutline();
	drawBarrel();
}

void PatrollingTurretHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for " + getName() + " poseDraw!" << std::endl;
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
			if (Renderer::isDebugDrawingEnabled(this->getName())) {
				drawPath();
			}
			break;
	}
}

void PatrollingTurretHazard::ghostDraw(float alpha) const {
	drawBody(alpha);
	drawOutline(alpha);
	drawBarrel(alpha);
}

void PatrollingTurretHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for " + getName() + " ghostDraw!" << std::endl;
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

inline void PatrollingTurretHazard::drawPath(float alpha) const {
	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);
	color = ColorMixer::mix(BackgroundRect::getBackColor(), color, alpha);
	const float radius = this->r / 2;
	const float lineWidth = 1.0f;

	//float* coordsAndColor = new float[((Circle::numOfSides+1 + 4) * routePosPairNum)*(2+4)];
	//unsigned int* indices = new unsigned int[(Circle::numOfSides*3 + 6) * routePosPairNum];
	//int startVertex = (Circle::numOfSides+1+4)*6 * i;
	//int startIndex  = (Circle::numOfSides*3+6) * i;

	for (int i = 0; i < routePosPairNum; i++) {
		float coordsAndColor[(Circle::numOfSides+1 + 4)*(2+4)];
		unsigned int indices[Circle::numOfSides*3 + 6];

		//circle
		coordsAndColor[0] = getRoutePosX(i);
		coordsAndColor[1] = getRoutePosY(i);
		coordsAndColor[2] = color.getRf();
		coordsAndColor[3] = color.getGf();
		coordsAndColor[4] = color.getBf();
		coordsAndColor[5] = color.getAf();
		for (int j = 1; j < Circle::numOfSides+1; j++) {
			coordsAndColor[j*6]   = getRoutePosX(i) + radius * cos((j-1) * (2*PI / Circle::numOfSides));
			coordsAndColor[j*6+1] = getRoutePosY(i) + radius * sin((j-1) * (2*PI / Circle::numOfSides));
			coordsAndColor[j*6+2] = color.getRf();
			coordsAndColor[j*6+3] = color.getGf();
			coordsAndColor[j*6+4] = color.getBf();
			coordsAndColor[j*6+5] = color.getAf();
		}

		for (int j = 0; j < Circle::numOfSides; j++) {
			indices[j*3]   = 0;
			indices[j*3+1] = j+1;
			indices[j*3+2] = (j+1) % Circle::numOfSides + 1;
		}

		//line
		SimpleVector2D dist = SimpleVector2D(getRoutePosX((i+1) % routePosPairNum) - getRoutePosX(i), getRoutePosY((i+1) % routePosPairNum) - getRoutePosY(i));
		SimpleVector2D distCW  = SimpleVector2D(dist.getAngle() - PI/2, lineWidth, true);
		//SimpleVector2D distCCW = SimpleVector2D(dist.getAngle() + PI/2, lineWidth, true);

		coordsAndColor[(Circle::numOfSides+1)    *6]   = getRoutePosX(i)                   + distCW.getXComp();
		coordsAndColor[(Circle::numOfSides+1)    *6+1] = getRoutePosY(i)                   + distCW.getYComp();
		coordsAndColor[(Circle::numOfSides+1 + 1)*6]   = getRoutePosX(i) + dist.getXComp() + distCW.getXComp();
		coordsAndColor[(Circle::numOfSides+1 + 1)*6+1] = getRoutePosY(i) + dist.getYComp() + distCW.getYComp();
		coordsAndColor[(Circle::numOfSides+1 + 2)*6]   = getRoutePosX(i) + dist.getXComp() - distCW.getXComp();
		coordsAndColor[(Circle::numOfSides+1 + 2)*6+1] = getRoutePosY(i) + dist.getYComp() - distCW.getYComp();
		coordsAndColor[(Circle::numOfSides+1 + 3)*6]   = getRoutePosX(i)                   - distCW.getXComp();
		coordsAndColor[(Circle::numOfSides+1 + 3)*6+1] = getRoutePosY(i)                   - distCW.getYComp();

		for (int j = 0; j < 4; j++) {
			coordsAndColor[(Circle::numOfSides+1 + j)*6+2] = color.getRf();
			coordsAndColor[(Circle::numOfSides+1 + j)*6+3] = color.getGf();
			coordsAndColor[(Circle::numOfSides+1 + j)*6+4] = color.getBf();
			coordsAndColor[(Circle::numOfSides+1 + j)*6+5] = color.getAf();
		}

		indices[Circle::numOfSides*3]     = (Circle::numOfSides+1);
		indices[Circle::numOfSides*3 + 1] = (Circle::numOfSides+1) + 1;
		indices[Circle::numOfSides*3 + 2] = (Circle::numOfSides+1) + 2;
		indices[Circle::numOfSides*3 + 3] = (Circle::numOfSides+1) + 2;
		indices[Circle::numOfSides*3 + 4] = (Circle::numOfSides+1) + 3;
		indices[Circle::numOfSides*3 + 5] = (Circle::numOfSides+1);

		Renderer::SubmitBatchedDraw(coordsAndColor, (Circle::numOfSides+1 + 4)*(2+4), indices, Circle::numOfSides*3 + 6);
		//would it make more sense to separate the circle and line draw calls? would it make more sense to group everything together for one call? dunno, but this seems easiest and cleanest
	}
}

CircleHazard* PatrollingTurretHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData& args) {
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
