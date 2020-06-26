#include "oldbignamedbulletpower.h"
#include "oldbignamedtankpower.h"

InteractionBoolHolder OldBigNamedBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	return { false, true };
}

void OldBigNamedBulletPower::initialize(Bullet* parent) {
	//nothing
}

void OldBigNamedBulletPower::removeEffects(Bullet* parent) {
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
