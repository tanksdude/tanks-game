#include "fire-named-power.h"

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



#include "constants.h"
#include <math.h>
#include "rng.h"

const double FireNamedTankPower::bulletAngleDeviation = PI/4;
const int FireNamedTankPower::bulletAmount = 4;

void FireNamedTankPower::initialize(Tank* parent) {
	//nothing
}

void FireNamedTankPower::removeEffects(Tank* parent) {
	//nothing
}

void FireNamedTankPower::additionalShooting(Tank* t, CannonPoint c) {
	for (int i = 0; i < bulletAmount; i++) {
		double tempAngle = (RNG::randFunc2()+RNG::randFunc2() - 1) * bulletAngleDeviation; //[-1,1] * deviation
		t->regularMakeBullet(t->r * cos(c.angle + t->velocity.getAngle() + tempAngle), t->r * sin(c.angle + t->velocity.getAngle() + tempAngle), c.angle + t->velocity.getAngle() + tempAngle);
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



#include "rng.h"

const double FireNamedBulletPower::maxBulletAcceleration = 3/32.0;
const double FireNamedBulletPower::minBulletAcceleration = 1/32.0;
const double FireNamedBulletPower::degradeAmount = .5;
const double FireNamedBulletPower::growAmount = 1.5/32.0; //TODO: need way to pass parameters into bulletpower constructor

InteractionBoolHolder FireNamedBulletPower::modifiedMovement(Bullet* b) {
	if (b->velocity.getMagnitude() > 0) {
		b->r += growAmount;
	} else if (b->velocity.getMagnitude() <= 0) {
		b->opaqueness -= degradeAmount;
	} /*else if (b->velocity < 0) {
		b->velocity.setMagnitude(0);
		b->acceleration = 0;
	}*/
	return { false };
}

InteractionBoolHolder FireNamedBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	if (b->velocity.getMagnitude() <= 0) {
		b->opaqueness -= degradeAmount;
		return { b->isDead(), false };
	} else {
		if (b->acceleration < 0) {
			b->velocity.changeMagnitude(b->acceleration);
			/*
			if (b->velocity.getMagnitude() < 0) {
				b->velocity.setMagnitude(0);
			}
			*/
		}
		return { false, false };
	}
}

double FireNamedBulletPower::getBulletAcceleration() const {
	return accelerationAmount;
}

void FireNamedBulletPower::initialize(Bullet* parent) {
	//nothing
}

void FireNamedBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

BulletPower* FireNamedBulletPower::makeDuplicate() const {
	return new FireNamedBulletPower(this->accelerationAmount);
}

TankPower* FireNamedBulletPower::makeTankPower() const {
	return new FireNamedTankPower();
}

FireNamedBulletPower::FireNamedBulletPower()
: FireNamedBulletPower(-1 * ((RNG::randFunc2()+RNG::randFunc2())/2 * (maxBulletAcceleration - minBulletAcceleration) + minBulletAcceleration)) {}
//accleration: [0,1] * accDiff + min

FireNamedBulletPower::FireNamedBulletPower(double acceleration) {
	timeLeft = 0;
	maxTime = -1;

	accelerationAmount = acceleration;

	modifiesMovement = true;
	modifiesCollisionWithWall = true;
}
