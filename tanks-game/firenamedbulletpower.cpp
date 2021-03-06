#include "firenamedbulletpower.h"
#include "firenamedtankpower.h"
#include "mylib.h"

const double FireNamedBulletPower::maxBulletAcceleration = 3/32.0;
const double FireNamedBulletPower::minBulletAcceleration = 1/32.0;
const double FireNamedBulletPower::degradeAmount = .5;
const double FireNamedBulletPower::growAmount = 1.5/32.0; //TODO: need way to pass parameters into bulletpower constructor

InteractionBoolHolder FireNamedBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
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

InteractionBoolHolder FireNamedBulletPower::modifiedMovement(Bullet* b) {
	if (b->velocity > 0) {
		b->r += growAmount;
	}
	else if (b->velocity == 0) {
		b->alpha -= degradeAmount;
	}
	else if (b->velocity < 0) {
		b->velocity = 0;
		b->acceleration = 0;
	}
	return { false };
}

double FireNamedBulletPower::getBulletAcceleration() {
	return accelerationAmount;
}

void FireNamedBulletPower::initialize(Bullet* parent) {
	//nothing
}

void FireNamedBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

BulletPower* FireNamedBulletPower::makeDuplicate() {
	return new FireNamedBulletPower(this->accelerationAmount);
}

TankPower* FireNamedBulletPower::makeTankPower() {
	return new FireNamedTankPower();
}

FireNamedBulletPower::FireNamedBulletPower()
: FireNamedBulletPower(-1 * ((randFunc2()+randFunc2())/2 * (maxBulletAcceleration - minBulletAcceleration) + minBulletAcceleration)) {}
//accleration: [0,1] * accDiff + min

FireNamedBulletPower::FireNamedBulletPower(double acceleration) {
	timeLeft = 0;
	maxTime = -1;

	accelerationAmount = acceleration;

	modifiesMovement = true;
	modifiesCollisionWithWall = true;
	overridesCollisionWithWall = true;
}
