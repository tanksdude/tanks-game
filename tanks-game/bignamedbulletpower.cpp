#include "bignamedbulletpower.h"
#include "bignamedtankpower.h"

PowerInteractionBoolHolder BigNamedBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	return { false, true };
}

void BigNamedBulletPower::initialize(Bullet* parent) {
	//nothing
}

void BigNamedBulletPower::removeEffects(Bullet* parent) {
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
