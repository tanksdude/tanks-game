#include "wallhackbulletpower.h"
#include "wallhacktankpower.h"

void WallhackBulletPower::initialize(Bullet* parent) {
	//nothing
}

void WallhackBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* WallhackBulletPower::makeTankPower() {
	return new WallhackTankPower();
}

InteractionBoolHolder WallhackBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	return { false, false };
}

WallhackBulletPower::WallhackBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;
}
