#pragma once
#include "speedtankpower.h"
#include "speedbulletpower.h"
#include "speedpower.h"

void SpeedTankPower::initialize(Tank* parent) {
	parent->giveMaxSpeed() *= 2;
}
void SpeedTankPower::removeEffects(Tank* parent) {
	parent->giveMaxSpeed() /= 2;
}

BulletPower* SpeedTankPower::makeBulletPower() {
	return new SpeedBulletPower();
}

SpeedTankPower::SpeedTankPower() {
	maxTime = 500;
	timeLeft = 500;
}

/*
void SpeedTankPower::modifiedMovement() = nothingFunction;

void SpeedTankPower::modifiedCollisionWithTank(Tank*) = nullptr;
void SpeedTankPower::modifiedCollisionWithWall(Wall*) = nullptr;
//void SpeedTankPower::modifiedCollisionWithPower(Power*) = nullptr;
//void SpeedTankPower::modifiedCollisionWithBullet(Bullet*) = nullptr;

void SpeedTankPower::modifiedShooting(Tank* parent) = nullptr;
void SpeedTankPower::modifiedTankDrawings(Tank* parent) = nullptr;

//double SpeedTankPower::getOffenseTier() { return 0; } //don't want it to be a variable because a function can change its value much easier
//double SpeedTankPower::getOffenseValue() { return 0; } //only one tier per power
//double SpeedTankPower::getDefenseTier() { return 0; }
//double SpeedTankPower::getDefenseValue() { return 0; } //return 0 by default, else 1 probably

*/