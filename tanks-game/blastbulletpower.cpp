#include "blastbulletpower.h"
#include "blasttankpower.h"
#include "collisionhandler.h"

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
