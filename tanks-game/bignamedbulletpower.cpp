#pragma once
#include "bignamedbulletpower.h"
#include "bignamedtankpower.h"
#include "powerfunctionhelper.h"
#include "constants.h"
#include "collisionhandler.h"

PowerInteractionBoolHolder BigNamedBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	return { false, true };
}

PowerInteractionBoolHolder BigNamedBulletPower::modifiedCollisionWithCircleHazard(Bullet* b, CircleHazard* ch) {
	return { false, true };
}

PowerInteractionBoolHolder BigNamedBulletPower::modifiedCollisionWithRectHazard(Bullet* b, RectHazard* rh) {
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

BigNamedBulletPower::BigNamedBulletPower(){
	timeLeft = 0;
	maxTime = -1;

	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;
	modifiesCollisionWithCircleHazard = true;
	modifiedCollisionWithCircleHazardCanWorkWithOthers = false;
	modifiesCollisionWithRectHazard = true;
	modifiedCollisionWithRectHazardCanWorkWithOthers = false;
}