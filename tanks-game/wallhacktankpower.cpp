#pragma once
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

PowerInteractionBoolHolder WallhackTankPower::modifiedCollisionWithWall(Tank* t, Wall* w) {
	return { PowerFunctionHelper::wallhackGenericTank(t, w), false };
}

WallhackTankPower::WallhackTankPower() {
	maxTime = 250;
	timeLeft = 250;

	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;
}


