#include "blastbulletpower.h"
#include "blasttankpower.h"
#include "collisionhandler.h"
#include "mylib.h"

const double BlastBulletPower::maxBulletAcceleration = 3/16.0;
const double BlastBulletPower::minBulletAcceleration = 1/16.0;
const double BlastBulletPower::degradeAmount = .25;

InteractionBoolHolder BlastBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	if (b->velocity == 0) {
		b->alpha -= degradeAmount;
		return { b->isDead(), false };
	} else {
		if (CollisionHandler::partiallyCollided(b, w)) {
			CollisionHandler::pushMovableAwayFromImmovable(b, w);

			b->acceleration = 0;
			b->velocity = 0;
		}
		return { false, false };
	}
}

InteractionBoolHolder BlastBulletPower::modifiedMovement(Bullet* b) {
	if (b->velocity == 0) {
		b->alpha -= degradeAmount;
	}
	else if (b->velocity < 0) {
		b->velocity = 0;
		b->acceleration = 0;
	}
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
: BlastBulletPower(-1 * ((randFunc2()+randFunc2())/2 * (maxBulletAcceleration - minBulletAcceleration) + minBulletAcceleration)) {}
//acceleration: [0,1] * accDiff + min

BlastBulletPower::BlastBulletPower(double acceleration) {
	timeLeft = 0;
	maxTime = -1;

	accelerationAmount = acceleration;

	modifiesMovement = true;
	modifiesCollisionWithWall = true;
	overridesCollisionWithWall = true;
}
