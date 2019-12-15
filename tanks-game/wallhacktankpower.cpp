#pragma once
class WallhackTankPower;

#include "wallhacktankpower.h"
#include "wallhackbulletpower.h"
#include "wallhackpower.h"

void WallhackTankPower::initialize(Tank* parent) {
	//parent->giveMaxSpeed() *= 2;
}
void WallhackTankPower::removeEffects(Tank* parent) {
	//parent->giveMaxSpeed() /= 2;
}

BulletPower* WallhackTankPower::makeBulletPower() {
	return new WallhackBulletPower();
}

WallhackTankPower::WallhackTankPower() {
	maxTime = 500;
	timeLeft = 500;
	modifiedCollisionWithWall = PowerFunctionHelper::wallhackGenericTank;
	addShootingPoints = nullptr;
}

/*
void WallhackTankPower::modifiedMovement() = nothingFunction;

void WallhackTankPower::modifiedCollisionWithTank(Tank*) = nullptr;
void WallhackTankPower::modifiedCollisionWithWall(Wall*) = nullptr;
//void WallhackTankPower::modifiedCollisionWithPower(Power*) = nullptr;
//void WallhackTankPower::modifiedCollisionWithBullet(Bullet*) = nullptr;

void WallhackTankPower::modifiedShooting(Tank* parent) = nullptr;
void WallhackTankPower::modifiedTankDrawings(Tank* parent) = nullptr;

//double WallhackTankPower::getOffenseTier() { return 0; } //don't want it to be a variable because a function can change its value much easier
//double WallhackTankPower::getOffenseValue() { return 0; } //only one tier per power
//double WallhackTankPower::getDefenseTier() { return 0; }
//double WallhackTankPower::getDefenseValue() { return 0; } //return 0 by default, else 1 probably

*/