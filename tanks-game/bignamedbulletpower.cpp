#pragma once
#include "bignamedbulletpower.h"
#include "bignamedtankpower.h"
#include "powerfunctionhelper.h"
#include "constants.h"
#include "collisionhandler.h"

bool BigNamedBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w, int index) {
	if (CollisionHandler::partiallyCollided(b, w)) {
		delete walls[index];
		walls.erase(walls.begin() + index);
	}
	return false;
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
}