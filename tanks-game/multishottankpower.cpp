#pragma once
#include "constants.h"
#include <vector>
#include "multishottankpower.h"
#include "multishotbulletpower.h"
#include "multishotpower.h"
#include "cannonpoint.h"
#include "powerfunctionhelper.h"

void MultishotTankPower::initialize(Tank* parent) {
	//deals with cannon points; nothing to initialize
}
void MultishotTankPower::removeEffects(Tank* parent) {
	//regenerate cannon points?
}

BulletPower* MultishotTankPower::makeBulletPower() {
	return new MultishotBulletPower();
}

void MultishotTankPower::addShootingPoints(Tank* t, std::vector<CannonPoint>* cannonPoints) {
	PowerFunctionHelper::equallySpacedCannonPoints(t, cannonPoints, MultishotPower::bulletConstant);
}

MultishotTankPower::MultishotTankPower() {
	maxTime = 500;
	timeLeft = 500;
	addsShootingPoints = true;
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