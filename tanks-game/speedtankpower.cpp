#pragma once
#include "speedtankpower.h"

void SpeedTankPower::initialize(Tank* parent) {
	//parent->maxSpeed *= 2;
}
void SpeedTankPower::removePower(Tank* parent) {
	//parent->maxSpeed /= 2;
}

void SpeedTankPower::modifiedMovement() { return; }

void SpeedTankPower::modifiedCollisionWithTank() { return; }
void SpeedTankPower::modifiedCollisionWithWall() { return; }
void SpeedTankPower::modifiedCollisionWithPower() { return; }
void SpeedTankPower::modifiedCollisionWithBullet() { return; }

void SpeedTankPower::modifiedShooting() { return; }
void SpeedTankPower::modifiedTankDrawings() { return; }

double SpeedTankPower::getOffenseTier() { return 0; } //don't want it to be a variable because a function can change its value much easier
double SpeedTankPower::getOffenseValue() { return 0; } //only one tier per power
double SpeedTankPower::getDefenseTier() { return 0; }
double SpeedTankPower::getDefenseValue() { return 0; } //return 0 by default, else 1 probably