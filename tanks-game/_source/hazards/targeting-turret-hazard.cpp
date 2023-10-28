#include "targeting-turret-hazard.h"

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

std::unordered_map<std::string, float> TargetingTurretHazard::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

TargetingTurretHazard::TargetingTurretHazard(double xpos, double ypos, double angle) : StationaryTurretHazard(xpos, ypos, angle) {
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

	//canAcceptPowers = false; //... true?
}

TargetingTurretHazard::TargetingTurretHazard(double xpos, double ypos, double angle, double radius) : TargetingTurretHazard(xpos, ypos, angle) {
	r = radius;
}

TargetingTurretHazard::~TargetingTurretHazard() {
	//delete[] stateMultiplier;
	//delete[] stateColors;
}

CircleHazard* TargetingTurretHazard::factory(const GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) [[likely]] {
		int count = args.getDataPortionLength(0);

		if (count >= 3) [[likely]] {
			const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
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
			targetingCount = 0;
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
	//targetingCount = 0;

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
		const CircleHazard* ch = HazardManager::getCircleHazard(i);
		if (ch->getGameID() != this->getGameID()) [[likely]] {
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
	return ColorMixer::mix(stateColors[currentState], stateColors[(currentState+1)%maxState], std::clamp<double>(targetingCount/(tickCycle*stateMultiplier[currentState]), 0, 1));
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
			std::cerr << "WARNING: unknown DrawingLayer for " + getName() + " draw!" << std::endl;
			[[fallthrough]];
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
	drawBody();
	drawOutline();
	drawBarrel();
}

void TargetingTurretHazard::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for " + getName() + " poseDraw!" << std::endl;
			[[fallthrough]];
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
	drawBody(alpha);
	drawOutline(alpha);
	drawBarrel(alpha);
}

void TargetingTurretHazard::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for " + getName() + " ghostDraw!" << std::endl;
			[[fallthrough]];
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

inline void TargetingTurretHazard::drawReticule(float alpha) const {
	if (!targeting) {
		return;
	}

	alpha = std::clamp<float>(alpha, 0, 1);
	alpha = alpha * alpha;

	ColorValueHolder color_outline = getReticuleColor();
	color_outline = ColorMixer::mix(BackgroundRect::getBackColor(), color_outline, alpha);
	const float lineWidth = 1.0f;
	//optional rotate angle: (-PI/2) * (targetingCount/(stateMultiplier[0] * tickCycle))

	float coordsAndColor_outline[(Circle::numOfSides*2 + 4*4*2)*(2+4)];
	for (int i = 0; i < Circle::numOfSides; i++) {
		coordsAndColor_outline[(i*2)  *6]   = targetingX + ((2*TANK_RADIUS) - lineWidth) * body_vertices[i+1].getXComp();
		coordsAndColor_outline[(i*2)  *6+1] = targetingY + ((2*TANK_RADIUS) - lineWidth) * body_vertices[i+1].getYComp();
		coordsAndColor_outline[(i*2+1)*6]   = targetingX + ((2*TANK_RADIUS) + lineWidth) * body_vertices[i+1].getXComp();
		coordsAndColor_outline[(i*2+1)*6+1] = targetingY + ((2*TANK_RADIUS) + lineWidth) * body_vertices[i+1].getYComp();

		coordsAndColor_outline[(i*2)  *6+2] = color_outline.getRf();
		coordsAndColor_outline[(i*2)  *6+3] = color_outline.getGf();
		coordsAndColor_outline[(i*2)  *6+4] = color_outline.getBf();
		coordsAndColor_outline[(i*2)  *6+5] = color_outline.getAf();
		coordsAndColor_outline[(i*2+1)*6+2] = color_outline.getRf();
		coordsAndColor_outline[(i*2+1)*6+3] = color_outline.getGf();
		coordsAndColor_outline[(i*2+1)*6+4] = color_outline.getBf();
		coordsAndColor_outline[(i*2+1)*6+5] = color_outline.getAf();
	}

	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0))  *6]   = targetingX + ( .75*(2*TANK_RADIUS) - lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0))  *6+1] = targetingY - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0+1))*6]   = targetingX + (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0+1))*6+1] = targetingY - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0+2))*6]   = targetingX + (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0+2))*6+1] = targetingY + lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0+3))*6]   = targetingX + ( .75*(2*TANK_RADIUS) - lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*0+3))*6+1] = targetingY + lineWidth;

	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1))  *6]   = targetingX + lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1))  *6+1] = targetingY + ( .75*(2*TANK_RADIUS) - lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1+1))*6]   = targetingX + lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1+1))*6+1] = targetingY + (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1+2))*6]   = targetingX - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1+2))*6+1] = targetingY + (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1+3))*6]   = targetingX - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*1+3))*6+1] = targetingY + ( .75*(2*TANK_RADIUS) - lineWidth);

	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2))  *6]   = targetingX - ( .75*(2*TANK_RADIUS) - lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2))  *6+1] = targetingY - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2+1))*6]   = targetingX - (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2+1))*6+1] = targetingY - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2+2))*6]   = targetingX - (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2+2))*6+1] = targetingY + lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2+3))*6]   = targetingX - ( .75*(2*TANK_RADIUS) - lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*2+3))*6+1] = targetingY + lineWidth;

	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3))  *6]   = targetingX - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3))  *6+1] = targetingY - ( .75*(2*TANK_RADIUS) - lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3+1))*6]   = targetingX - lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3+1))*6+1] = targetingY - (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3+2))*6]   = targetingX + lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3+2))*6+1] = targetingY - (1.25*(2*TANK_RADIUS) + lineWidth);
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3+3))*6]   = targetingX + lineWidth;
	coordsAndColor_outline[(Circle::numOfSides*2 + (4*3+3))*6+1] = targetingY - ( .75*(2*TANK_RADIUS) - lineWidth);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			coordsAndColor_outline[(Circle::numOfSides*2 + (4*i+j))*6+2] = color_outline.getRf();
			coordsAndColor_outline[(Circle::numOfSides*2 + (4*i+j))*6+3] = color_outline.getGf();
			coordsAndColor_outline[(Circle::numOfSides*2 + (4*i+j))*6+4] = color_outline.getBf();
			coordsAndColor_outline[(Circle::numOfSides*2 + (4*i+j))*6+5] = color_outline.getAf();
		}
	}

	unsigned int indices_outline[Circle::numOfSides*6 + 4*6];
	std::copy(StationaryTurretHazard::outline_indices, StationaryTurretHazard::outline_indices + Circle::numOfSides*6, indices_outline);

	for (int i = 0; i < 4; i++) {
		indices_outline[Circle::numOfSides*6 + i*6]   = (Circle::numOfSides*2 + 4*i);
		indices_outline[Circle::numOfSides*6 + i*6+1] = (Circle::numOfSides*2 + 4*i+1);
		indices_outline[Circle::numOfSides*6 + i*6+2] = (Circle::numOfSides*2 + 4*i+2);
		indices_outline[Circle::numOfSides*6 + i*6+3] = (Circle::numOfSides*2 + 4*i+2);
		indices_outline[Circle::numOfSides*6 + i*6+4] = (Circle::numOfSides*2 + 4*i+3);
		indices_outline[Circle::numOfSides*6 + i*6+5] = (Circle::numOfSides*2 + 4*i);
	}

	Renderer::SubmitBatchedDraw(coordsAndColor_outline, (Circle::numOfSides*2 + 4*4*2)*(2+4), indices_outline, Circle::numOfSides*6 + 4*6);
}

CircleHazard* TargetingTurretHazard::randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData& args) {
	int attempts = 0;
	CircleHazard* randomized = nullptr;
	double xpos, ypos, angle;

	int count = 0;
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);
	}
	if (count >= 1) {
		const double* arr = static_cast<const double*>(args.getDataPortion(0).get());
		angle = arr[0];
	} else {
		angle = RNG::randFunc() * (2*PI);
	}

	do {
		xpos = RNG::randFunc() * (area_width - 2*(TANK_RADIUS/2)) + (x_start + (TANK_RADIUS/2));
		ypos = RNG::randFunc() * (area_height - 2*(TANK_RADIUS/2)) + (y_start + (TANK_RADIUS/2));
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
