#pragma once
#include "constants.h"
#include <vector>
#include "multishottankpower.h"
#include "multishotbulletpower.h"
#include "multishotpower.h"
#include "cannonpoint.h"

void MultishotTankPower::addCannonPointsDummy(Tank*, std::vector<CannonPoint>* cannonPoints) {
	for (int i = cannonPoints->size() - 1; i >= 0; i--) {
		int end = (i + 1) % cannonPoints->size();
		double angle_diff;
		if (end == 0) {
			angle_diff = 2*PI - (cannonPoints->at(i).angle - cannonPoints->at(end).angle);
		} else {
			angle_diff = cannonPoints->at(end).angle - cannonPoints->at(i).angle;
		}

		for (int j = 0; j < MultishotPower::bulletConstant - 1; j++) {
			double newAngle = angle_diff/MultishotPower::bulletConstant * (j+1);
			CannonPoint temp = CannonPoint(newAngle + cannonPoints->at(i).angle);
			cannonPoints->insert(cannonPoints->begin() + i + j + 1, temp);

			if (end != 0) {
				end++;
			}
		}
	}
}

void MultishotTankPower::initialize(Tank* parent) {
	//deals with cannon points; nothing to initialize
}
void MultishotTankPower::removeEffects(Tank* parent) {
	//regenerate cannon points?
}

BulletPower* MultishotTankPower::makeBulletPower() {
	return new MultishotBulletPower();
}

MultishotTankPower::MultishotTankPower() {
	maxTime = 500;
	timeLeft = 500;
	modifiedCollisionWithWall = nullptr;
	addShootingPoints = MultishotTankPower::addCannonPointsDummy;
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