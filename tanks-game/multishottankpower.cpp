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
