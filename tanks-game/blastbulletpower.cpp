#include "blastbulletpower.h"
#include "blasttankpower.h"
#include "collisionhandler.h"
#include "mylib.h"

const double BlastBulletPower::maxBulletAcceleration = 3/16.0;
const double BlastBulletPower::minBulletAcceleration = 1/16.0;
const double BlastBulletPower::degradeAmount = .25;

PowerInteractionBoolHolder BlastBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
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

PowerInteractionBoolHolder BlastBulletPower::modifiedMovement(Bullet* b) {
	if (b->velocity == 0) {
		b->alpha -= degradeAmount;
	}
	else if (b->velocity < 0) {
		b->velocity = 0;
		b->acceleration = 0;
	}
	return { false };
}

double BlastBulletPower::getBulletAcceleration() {
	return accelerationAmount;
}

void BlastBulletPower::initialize(Bullet* b) {
	//nothing
}

void BlastBulletPower::removeEffects(Bullet* b) {
	//nothing
}

BulletPower* BlastBulletPower::makeDuplicate() {
	return new BlastBulletPower(this->accelerationAmount);
}

TankPower* BlastBulletPower::makeTankPower() {
	return new BlastTankPower();
}

BlastBulletPower::BlastBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	accelerationAmount = -1 * ((randFunc2()+randFunc2())/2 * (maxBulletAcceleration - minBulletAcceleration) + minBulletAcceleration); //[0,1] * accDiff + min

	modifiesMovement = true;
	modifiesCollisionWithWall = true;
	overridesCollisionWithWall = true;
}

//is there a way to avoid the copy-and-paste? yes. will I do it? no.
BlastBulletPower::BlastBulletPower(double acceleration) {
	timeLeft = 0;
	maxTime = -1;

	accelerationAmount = acceleration;

	modifiesMovement = true;
	modifiesCollisionWithWall = true;
	overridesCollisionWithWall = true;
}
