#pragma once
class WallhackTankPower;

#include "wallhacktankpower.h"
#include "wallhackbulletpower.h"
#include "wallhackpower.h"

void WallhackTankPower::initialize(Tank* parent) {
	//nothing
}
void WallhackTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* WallhackTankPower::makeBulletPower() {
	return new WallhackBulletPower();
}

bool WallhackTankPower::modifiedCollisionWithWall(Tank* t, Wall* w) {
	return PowerFunctionHelper::wallhackGenericTank(t, w);
}

WallhackTankPower::WallhackTankPower() {
	maxTime = 250;
	timeLeft = 250;

	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;
}


