#pragma once
#include "invinciblenamedpower.h"
#include "invinciblenamedtankpower.h"
#include "invinciblenamedbulletpower.h"

const bool InvincibleNamedPower::canBeInARandomLevel = true;

TankPower* InvincibleNamedPower::makeTankPower() {
	return new InvincibleNamedTankPower();
}

BulletPower* InvincibleNamedPower::makeBulletPower() {
	return new InvincibleNamedBulletPower();
}

/*
HazardPower* InvincibleNamedPower::makeHazardPower(){
	return new InvincibleNamedHazardPower();
}
*/

Power* InvincibleNamedPower::factory() {
	return new InvincibleNamedPower();
}

InvincibleNamedPower::InvincibleNamedPower() {
	//nope
}

InvincibleNamedPower::~InvincibleNamedPower() {
	//unlikely to ever need these
}