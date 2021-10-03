#include "blastpower.h"

TankPower* BlastPower::makeTankPower() const {
	return new BlastTankPower();
}

BulletPower* BlastPower::makeBulletPower() const {
	return new BlastBulletPower();
}

/*
HazardPower* BlastPower::makeHazardPower() const {
	return new BlastHazardPower();
}
*/

Power* BlastPower::factory() {
	return new BlastPower();
}

BlastPower::BlastPower() {
	return;
}



#include "mylib.h"
#include "constants.h"
#include <math.h>
#include "rng.h"

const double BlastTankPower::bulletAngleDeviation = PI/3;
const int BlastTankPower::bulletAmount = 16;

void BlastTankPower::initialize(Tank* parent) {
	//nothing
}

void BlastTankPower::removeEffects(Tank* parent) {
	//nothing
}

void BlastTankPower::additionalShooting(Tank* t, CannonPoint c) {
	for (int i = 0; i < bulletAmount; i++) {
		double tempAngle = (RNG::randFunc2()*2 - 1) * bulletAngleDeviation; //[-1,1] * deviation
		t->regularMakeBullet(t->r * cos(c.angle + t->velocity.getAngle() + tempAngle), t->r * sin(c.angle + t->velocity.getAngle() + tempAngle), c.angle + t->velocity.getAngle() + tempAngle);
	}
}

BulletPower* BlastTankPower::makeBulletPower() const {
	return new BlastBulletPower();
}

BlastTankPower::BlastTankPower() {
	maxTime = 500;
	timeLeft = 500;

	modifiesAdditionalShooting = true;
	overridesAdditionalShooting = true;
}



#include "collisionhandler.h"
#include "rng.h"

const double BlastBulletPower::maxBulletAcceleration = 3/16.0;
const double BlastBulletPower::minBulletAcceleration = 1/16.0;
const double BlastBulletPower::degradeAmount = .25;

InteractionBoolHolder BlastBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	if (b->velocity.getMagnitude() <= 0) {
		b->alpha -= degradeAmount;
		return { b->isDead(), false };
	} else {
		if (CollisionHandler::partiallyCollided(b, w)) {
			CollisionHandler::pushMovableAwayFromImmovable(b, w);

			b->acceleration = 0;
			b->velocity.setMagnitude(0);
		}
		return { false, false };
	}
}

InteractionBoolHolder BlastBulletPower::modifiedMovement(Bullet* b) {
	if (b->velocity.getMagnitude() <= 0) {
		b->alpha -= degradeAmount;
	} /*else if (b->velocity < 0) {
		b->velocity.setMagnitude(0);
		b->acceleration = 0;
	}*/
	return { false };
}

double BlastBulletPower::getBulletAcceleration() const {
	return accelerationAmount;
}

void BlastBulletPower::initialize(Bullet* parent) {
	//nothing
}

void BlastBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

BulletPower* BlastBulletPower::makeDuplicate() const {
	return new BlastBulletPower(this->accelerationAmount);
}

TankPower* BlastBulletPower::makeTankPower() const {
	return new BlastTankPower();
}

BlastBulletPower::BlastBulletPower()
: BlastBulletPower(-1 * ((RNG::randFunc2()+RNG::randFunc2())/2 * (maxBulletAcceleration - minBulletAcceleration) + minBulletAcceleration)) {}
//acceleration: [0,1] * accDiff + min

BlastBulletPower::BlastBulletPower(double acceleration) {
	timeLeft = 0;
	maxTime = -1;

	accelerationAmount = acceleration;

	modifiesMovement = true;
	modifiesCollisionWithWall = true;
	overridesCollisionWithWall = true;
}
