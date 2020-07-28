#include "wallhacktankpower.h"
#include "wallhackbulletpower.h"

void WallhackTankPower::initialize(Tank* parent) {
	//nothing
}

void WallhackTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* WallhackTankPower::makeBulletPower() const {
	return new WallhackBulletPower();
}

InteractionBoolHolder WallhackTankPower::modifiedCollisionWithWall(Tank* t, Wall* w) {
	return { false, false };
}

WallhackTankPower::WallhackTankPower() {
	maxTime = 250;
	timeLeft = 250;

	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;
}
