#pragma once
#include "devlonginvinciblenamedpower.h"
#include "devlonginvinciblenamedtankpower.h"
#include "devlonginvinciblenamedbulletpower.h"

const bool DevLongInvincibleNamedPower::canBeInARandomLevel = false;

TankPower* DevLongInvincibleNamedPower::makeTankPower() {
	return new DevLongInvincibleNamedTankPower();
}

BulletPower* DevLongInvincibleNamedPower::makeBulletPower() {
	return new DevLongInvincibleNamedBulletPower();
}

/*
HazardPower* DevLongInvincibleNamedPower::makeHazardPower(){
	return new DevLongInvincibleNamedHazardPower();
}
*/

Power* DevLongInvincibleNamedPower::factory() {
	return new DevLongInvincibleNamedPower();
}

DevLongInvincibleNamedPower::DevLongInvincibleNamedPower() {
	return;
}

DevLongInvincibleNamedPower::~DevLongInvincibleNamedPower() {
	return;
}