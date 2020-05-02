#pragma once
#include "megadeathbulletpower.h"
#include "megadeathtankpower.h"
#include "powerfunctionhelper.h"
#include "constants.h"
#include "collisionhandler.h"

const double MegaDeathBulletPower::destroyWallTier = DESTRUCTION_TIER;
const double MegaDeathBulletPower::bulletSizeMultiplierPerTick = 65.0/64.0;

void MegaDeathBulletPower::powerTick(Bullet* b) {
	if (getOffenseTier(b) >= destroyWallTier) {
		modifiedCollisionWithWallCanWorkWithOthers = false;
	}
}

PowerInteractionBoolHolder MegaDeathBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	if (getOffenseTier(b) >= destroyWallTier) {
		return { false, true };
	} else {
		return { CollisionHandler::partiallyCollided(b, w), false };
	}
}

void MegaDeathBulletPower::modifiedMovement(Bullet* b) {
	b->r *= bulletSizeMultiplierPerTick;
}

double MegaDeathBulletPower::getOffenseTier(Bullet* b) {
	double value = b->r / (Bullet::default_radius*4) * destroyWallTier;
	//return (value >= destroyWallTier ? floor(value) : 0); //this is what I originally wanted in JS Tanks, I think, but in practice isn't preferable
	return value;
}

double MegaDeathBulletPower::getDefenseTier(Bullet* b) {
	double value = b->r / (Bullet::default_radius*4) * destroyWallTier;
	//return (value >= destroyWallTier ? floor(value) : 0);
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