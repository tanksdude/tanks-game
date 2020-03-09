#pragma once
#include "invinciblenamedtankpower.h"
#include "invinciblenamedbulletpower.h"
#include "invinciblenamedpower.h"

//TODO: get better solution for modifying tank internals
//system would need to get all tankpowers on tank, undo what the power wants, then redo the effects the power wants
//(undo step required because two /=2 steps shouldn't equal a /=4, unless stacking)
void InvincibleNamedTankPower::initialize(Tank* parent) {
	//nothing
	//parent->maxSpeed /= 2;
	//parent->acceleration /= 2;
}
void InvincibleNamedTankPower::removeEffects(Tank* parent) {
	//nothing
	//parent->maxSpeed *= 2;
	//parent->acceleration *= 2;
}

BulletPower* InvincibleNamedTankPower::makeBulletPower() {
	return new InvincibleNamedBulletPower();
}

InvincibleNamedTankPower::InvincibleNamedTankPower() {
	maxTime = 500;
	timeLeft = 500;
}
