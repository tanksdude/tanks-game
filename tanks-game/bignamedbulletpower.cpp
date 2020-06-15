#include "bignamedbulletpower.h"
#include "bignamedtankpower.h"

PowerInteractionBoolHolder BigNamedBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	return { false, true };
}

void BigNamedBulletPower::initialize(Bullet* b) {
	//nothing
}

void BigNamedBulletPower::removeEffects(Bullet * b) {
	//nothing
}

TankPower* BigNamedBulletPower::makeTankPower() {
	return new BigNamedTankPower();
}

BigNamedBulletPower::BigNamedBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;

	bulletRadiusStacks = true;
}
