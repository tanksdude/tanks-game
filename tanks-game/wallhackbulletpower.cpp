#pragma once
class WallhackBulletPower;

#include "wallhackbulletpower.h"
#include "wallhacktankpower.h"
#include "wallhackpower.h"
#include "powerfunctionhelper.h"

void WallhackBulletPower::initialize(Bullet*) { return; }
void WallhackBulletPower::removeEffects(Bullet*) { return; }

TankPower* WallhackBulletPower::makeTankPower() {
	return new WallhackTankPower();
}

WallhackBulletPower::WallhackBulletPower(){
	timeLeft = 0;
	maxTime = -1;
	modifiedCollisionWithWall = PowerFunctionHelper::wallhackGenericBullet;
}