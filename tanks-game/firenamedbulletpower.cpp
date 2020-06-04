#include "firenamedbulletpower.h"
#include "firenamedtankpower.h"

const double FireNamedBulletPower::degradeAmount = .5;
const double FireNamedBulletPower::growAmount = 1.5/32.0; //TODO: need way to pass parameters into bulletpower constructor

PowerInteractionBoolHolder FireNamedBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	if (b->velocity <= 0) {
		b->alpha -= degradeAmount;
		return { (b->alpha <= 0), false };
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

void FireNamedBulletPower::modifiedMovement(Bullet* b) {
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
}

void FireNamedBulletPower::initialize(Bullet* b) {
	//nothing
}

void FireNamedBulletPower::removeEffects(Bullet* b) {
	//nothing
}

TankPower* FireNamedBulletPower::makeTankPower() {
	return new FireNamedTankPower();
}

FireNamedBulletPower::FireNamedBulletPower(){
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
	modifiesCollisionWithWall = true;
	overridesCollisionWithWall = true;
}
