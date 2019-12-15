#pragma once
class WallhackBulletPower;

#include "wallhackbulletpower.h"
#include "wallhacktankpower.h"
#include "wallhackpower.h"
#include "powerfunctionhelper.h"

void WallhackBulletPower::initialize(Bullet*) { return; }
void WallhackBulletPower::removeEffects(Bullet*) { return; }

void WallhackBulletPower::modifiedCollisionWithWall(Wall* w) {
	return;
}

TankPower* WallhackBulletPower::makeTankPower() {
	return new WallhackTankPower();
}

WallhackBulletPower::WallhackBulletPower(){
	test = PowerFunctionHelper::wallhackGenericWall;
}