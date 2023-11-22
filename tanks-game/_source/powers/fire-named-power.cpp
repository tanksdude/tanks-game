#include "fire-named-power.h"

#include "../constants.h"

const double FireNamedPower::bulletAngleDeviation = PI/4;
const int FireNamedPower::bulletAmount = 4;

const double FireNamedPower::maxBulletAcceleration = 3/32.0;
const double FireNamedPower::minBulletAcceleration = 1/32.0;
const double FireNamedPower::degradeAmount = .5;
const double FireNamedPower::growAmount = 1.5/32.0; //JS: [1/32, 5/32] //TODO: need way to pass parameters into bulletpower constructor

std::unordered_map<std::string, float> FireNamedPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "supermix", 1.0f });
	weights.insert({ "supermix-vanilla", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

TankPower* FireNamedPower::makeTankPower() const {
	return new FireNamedTankPower();
}

BulletPower* FireNamedPower::makeBulletPower() const {
	return new FireNamedBulletPower();
}

/*
HazardPower* FireNamedPower::makeHazardPower() const {
	return new FireNamedHazardPower();
}
*/

Power* FireNamedPower::factory() {
	return new FireNamedPower();
}

FireNamedPower::FireNamedPower() {
	return;
}



#include "../rng.h"

void FireNamedTankPower::additionalShooting(Tank* t, const CannonPoint& c, const ExtraCannonPoint& c2) {
	for (int i = 0; i < FireNamedPower::bulletAmount; i++) {
		double tempAngle = (GameRNG::randFunc()+GameRNG::randFunc() - 1) * FireNamedPower::bulletAngleDeviation; //[-1,1) * deviation
		t->defaultMakeBullet(t->velocity.getAngle() + c.angleFromCenter + c2.angleFromCenter + tempAngle, c2.angleFromEdge);
	}
}

BulletPower* FireNamedTankPower::makeBulletPower() const {
	return new FireNamedBulletPower();
}

FireNamedTankPower::FireNamedTankPower() {
	maxTime = 500;
	timeLeft = 500;
	//JS: maxTime = 1000

	modifiesAdditionalShooting = true;
	overridesAdditionalShooting = true;
}



#include "../rng.h"

InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> FireNamedBulletPower::modifiedCollisionWithWall(const Bullet* b, const Wall* w) {
	if (b->velocity.getMagnitude() <= 0) {
		return { false, false, new BulletUpdateStruct(0,0,0,0,0, -FireNamedPower::degradeAmount), nullptr };
	} else {
		if (b->acceleration < 0) {
			return { false, false, new BulletUpdateStruct(0,0,0, b->acceleration, 0,0), nullptr };
		}
		return { false, false, nullptr, nullptr };
	}
}

BulletPower* FireNamedBulletPower::makeDuplicate() const {
	return new FireNamedBulletPower(this->accelerationAmount);
}

TankPower* FireNamedBulletPower::makeTankPower() const {
	return new FireNamedTankPower();
}

FireNamedBulletPower::FireNamedBulletPower()
: FireNamedBulletPower(-1 * ((GameRNG::randFunc()+GameRNG::randFunc())/2 * (FireNamedPower::maxBulletAcceleration - FireNamedPower::minBulletAcceleration) + FireNamedPower::minBulletAcceleration)) {}
//accleration: [0,1) * accDiff + min

FireNamedBulletPower::FireNamedBulletPower(double acceleration) {
	timeLeft = 0;
	maxTime = -1;

	accelerationAmount = acceleration;

	modifiesCollisionWithWall = true;

	bulletRadiusGrowMultiplies_Moving = false;
}
