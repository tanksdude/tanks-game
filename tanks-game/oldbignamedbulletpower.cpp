#include "oldbignamedbulletpower.h"
#include "oldbignamedtankpower.h"

PowerInteractionBoolHolder OldBigNamedBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	return { false, true };
}

void OldBigNamedBulletPower::initialize(Bullet* b) {
	//nothing
}

void OldBigNamedBulletPower::removeEffects(Bullet * b) {
	//nothing
}

TankPower* OldBigNamedBulletPower::makeTankPower() {
	return new OldBigNamedTankPower();
}

OldBigNamedBulletPower::OldBigNamedBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;

	bulletSpeedStacks = true;
	bulletRadiusStacks = true;
}