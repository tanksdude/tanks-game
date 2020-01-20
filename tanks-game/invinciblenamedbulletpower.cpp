#pragma once
#include "invinciblenamedbulletpower.h"
#include "invinciblenamedtankpower.h"
#include "powerfunctionhelper.h"
#include "constants.h"

void InvincibleNamedBulletPower::initialize(Bullet* b) {
	//nothing
}

void InvincibleNamedBulletPower::removeEffects(Bullet * b) {
	//nothing
}

TankPower* InvincibleNamedBulletPower::makeTankPower() {
	return new InvincibleNamedTankPower();
}

InvincibleNamedBulletPower::InvincibleNamedBulletPower(){
	timeLeft = 0;
	maxTime = -1;
}