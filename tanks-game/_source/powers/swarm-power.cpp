#include "swarm-power.h"

#include "../constants.h"

const double SwarmPower::initialAngleDiff = 15 * (PI/180);

const double SwarmPower::homingStrength = (2*PI) / 32;

std::unordered_map<std::string, float> SwarmPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 1.0f });
	weights.insert({ "random-vanilla", 0.5f });
	weights.insert({ "random", 0.25f });
	return weights;
}

TankPower* SwarmPower::makeTankPower() const {
	return new SwarmTankPower();
}

BulletPower* SwarmPower::makeBulletPower() const {
	return new SwarmBulletPower();
}

Power* SwarmPower::factory() {
	return new SwarmPower();
}

SwarmPower::SwarmPower() {
	return;
}



void SwarmTankPower::additionalShooting(Tank* t, const CannonPoint& c, const ExtraCannonPoint& c2) {
	if (leftRightToggle) {
		t->defaultMakeBullet(t->velocity.getAngle() + c.angleFromCenter + c2.angleFromCenter, c2.angleFromEdge - SwarmPower::initialAngleDiff);
	} else {
		t->defaultMakeBullet(t->velocity.getAngle() + c.angleFromCenter + c2.angleFromCenter, c2.angleFromEdge + SwarmPower::initialAngleDiff);
	}
	leftRightToggle = !leftRightToggle;
}

BulletPower* SwarmTankPower::makeBulletPower() const {
	return new SwarmBulletPower();
}

SwarmTankPower::SwarmTankPower() {
	maxTime = 500;
	timeLeft = 500;

	modifiesAdditionalShooting = true;
	overridesAdditionalShooting = true;

	leftRightToggle = false;
}



#include "../constants.h"
#include <cmath>

#include "../tank-manager.h"

InteractionBoolHolder SwarmBulletPower::modifiedMovement(Bullet* b) {
	const Tank* parentTank = TankManager::getTankByID(b->getParentID());
	const float interiorRadius = parentTank->r * 2;
	const SimpleVector2D distToTank = SimpleVector2D(parentTank->x - b->x, parentTank->y - b->y);
	if (distToTank.getMagnitude() <= interiorRadius) {
		//don't do anything
		return { false };
	}

	//note: another way of doing this is to aim at the points perpendicular instead of tangent
	//doesn't work as well so it wasn't done

	//from: https://en.wikipedia.org/wiki/Tangent_lines_to_circles#With_analytic_geometry

	const float r = interiorRadius;
	const float d0Squared = distToTank.getMagnitude() * distToTank.getMagnitude();
	const float x0 = b->x - parentTank->x;
	const float y0 = b->y - parentTank->y;

	const float point1XFromTank = ((r*r) / d0Squared) * x0   +   (r / d0Squared * sqrt(d0Squared - (r*r))) * -y0;
	const float point1YFromTank = ((r*r) / d0Squared) * y0   +   (r / d0Squared * sqrt(d0Squared - (r*r))) * x0;
	const float point2XFromTank = ((r*r) / d0Squared) * x0   -   (r / d0Squared * sqrt(d0Squared - (r*r))) * -y0;
	const float point2YFromTank = ((r*r) / d0Squared) * y0   -   (r / d0Squared * sqrt(d0Squared - (r*r))) * x0;

	const SimpleVector2D distToPoint1 = SimpleVector2D(point1XFromTank - x0, point1YFromTank - y0); //SimpleVector2D((parentTank->x + point1XFromTank) - b->x, (parentTank->y + point1YFromTank) - b->y);
	const SimpleVector2D distToPoint2 = SimpleVector2D(point2XFromTank - x0, point2YFromTank - y0); //SimpleVector2D((parentTank->x + point2XFromTank) - b->x, (parentTank->y + point2YFromTank) - b->y);
	const float angleToPoint1 = SimpleVector2D::angleBetween(distToPoint1, b->velocity);
	const float angleToPoint2 = SimpleVector2D::angleBetween(distToPoint2, b->velocity);

	//copied from PowerFunctionHelper::homingGenericMove()
	if (abs(angleToPoint1) < abs(angleToPoint2)) {
		if (abs(angleToPoint1) < SwarmPower::homingStrength) {
			//small angle adjustment needed
			b->velocity.setAngle(distToPoint1.getAngle());
		} else {
			//large angle adjustment needed
			if (angleToPoint1 < 0) {
				b->velocity.changeAngle(SwarmPower::homingStrength);
			} else {
				b->velocity.changeAngle(-SwarmPower::homingStrength);
			}
		}
	} else {
		if (abs(angleToPoint2) < SwarmPower::homingStrength) {
			//small angle adjustment needed
			b->velocity.setAngle(distToPoint2.getAngle());
		} else {
			//large angle adjustment needed
			if (angleToPoint2 < 0) {
				b->velocity.changeAngle(SwarmPower::homingStrength);
			} else {
				b->velocity.changeAngle(-SwarmPower::homingStrength);
			}
		}
	}

	return { false };
}

TankPower* SwarmBulletPower::makeTankPower() const {
	return new SwarmTankPower();
}

SwarmBulletPower::SwarmBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
}
