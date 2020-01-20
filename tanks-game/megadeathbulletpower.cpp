#pragma once
#include "megadeathbulletpower.h"
#include "megadeathtankpower.h"
#include "powerfunctionhelper.h"
#include "constants.h"
#include "collisionhandler.h"

PowerInteractionBoolHolder MegaDeathBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	if (getOffenseTier(b) >= 2) {
		return { false, true };
	} else {
		return { CollisionHandler::partiallyCollided(b, w), false };
	}
}

void MegaDeathBulletPower::modifiedMovement(Bullet* b) {
	//b->velocity *= 1 + 1.0/64;
	b->velocity += 1.0/64;
	b->r *= 65.0/64;
}

double MegaDeathBulletPower::getDefenseTier(Bullet* b) {
	double value = b->r / Tank::default_radius * 2;
	return value;
}

double MegaDeathBulletPower::getOffenseTier(Bullet* b) {
	double value = b->r / Tank::default_radius * 2;
	if (value >= 2) {
		modifiedCollisionWithWallCanWorkWithOthers = false;
	}
	return value;
}

void MegaDeathBulletPower::initialize(Bullet* b) {
	//nothing
}

void MegaDeathBulletPower::removeEffects(Bullet* b) {
	//nothing
}

TankPower* MegaDeathBulletPower::makeTankPower() {
	return new MegaDeathTankPower();
}

MegaDeathBulletPower::MegaDeathBulletPower(){
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
	modifiesCollisionWithWall = true;
	//modifiedCollisionWithWallCanWorkWithOthers = false;
}