#pragma once
#include "wallhackbulletpower.h"
#include "wallhacktankpower.h"
#include "wallhackpower.h"

void WallhackBulletPower::initialize(Bullet*) { return; }
void WallhackBulletPower::removeEffects(Bullet*) { return; }

TankPower* WallhackBulletPower::makeTankPower() {
	return new WallhackTankPower();
}

PowerInteractionBoolHolder WallhackBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	return { false, false };
}

WallhackBulletPower::WallhackBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;
}
