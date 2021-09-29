#include "grenadebulletpower.h"
#include "grenadetankpower.h"

const double GrenadeBulletPower::degradeAmount = .875;

InteractionBoolHolder GrenadeBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	if (b->velocity.getMagnitude() <= 0) {
		b->alpha -= degradeAmount;
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

InteractionBoolHolder GrenadeBulletPower::modifiedMovement(Bullet* b) {
	if (b->velocity.getMagnitude() <= 0) {
		b->alpha -= degradeAmount;
		b->r *= 65/64.0;
	} /*else if (b->velocity.getMagnitude() < 0) {
		b->velocity.setMagnitude(0);
		b->acceleration = 0;
	}*/
	return { false };
}

void GrenadeBulletPower::initialize(Bullet* parent) {
	//nothing
}

void GrenadeBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* GrenadeBulletPower::makeTankPower() const {
	return new GrenadeTankPower();
}

GrenadeBulletPower::GrenadeBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
	modifiesCollisionWithWall = true;
	overridesCollisionWithWall = true;
}
