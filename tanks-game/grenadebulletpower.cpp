#include "grenadebulletpower.h"
#include "grenadetankpower.h"

const double GrenadeBulletPower::degradeAmount = .875;

PowerInteractionBoolHolder GrenadeBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	if (b->velocity <= 0) {
		b->alpha -= degradeAmount;
		return { b->isDead(), false };
	} else {
		if (b->acceleration < 0) {
			b->velocity += b->acceleration;
			if (b->velocity < 0) {
				b->velocity = 0;
			}
		}
		return { false, false };
	}
}

PowerInteractionBoolHolder GrenadeBulletPower::modifiedMovement(Bullet* b) {
	if (b->velocity == 0) {
		b->alpha -= degradeAmount;
		b->r *= 65/64.0;
	}
	else if (b->velocity < 0) {
		b->velocity = 0;
		b->acceleration = 0;
	}
	return { false };
}

void GrenadeBulletPower::initialize(Bullet* parent) {
	//nothing
}

void GrenadeBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* GrenadeBulletPower::makeTankPower() {
	return new GrenadeTankPower();
}

GrenadeBulletPower::GrenadeBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
	modifiesCollisionWithWall = true;
	overridesCollisionWithWall = true;
}
