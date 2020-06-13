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

void BlastBulletPower::modifiedMovement(Bullet* b) {
	if (b->velocity == 0) {
		b->alpha -= degradeAmount;
	}
	else if (b->velocity < 0) {
		b->velocity = 0;
		b->acceleration = 0;
	}
}

double BlastBulletPower::getBulletAcceleration() {
	return -1 * ((randFunc2()+randFunc2())/2 * (maxBulletAcceleration - minBulletAcceleration) + minBulletAcceleration); //[0,1] * accDiff + min
}

void BlastBulletPower::initialize(Bullet* b) {
	//nothing
}

void BlastBulletPower::removeEffects(Bullet* b) {
	//nothing
}

TankPower* BlastBulletPower::makeTankPower() {
	return new BlastTankPower();
}

BlastBulletPower::BlastBulletPower(){
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
	modifiesCollisionWithWall = true;
	overridesCollisionWithWall = true;
}
