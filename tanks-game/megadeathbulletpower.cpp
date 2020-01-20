#pragma once
#include "megadeathbulletpower.h"
#include "megadeathtankpower.h"
#include "powerfunctionhelper.h"
#include "constants.h"
#include "collisionhandler.h"
//#include "bignamedbulletpower.h" //include if it has the tier needed for destroying walls as static (could put it in constants.h)

const double MegaDeathBulletPower::destroyWallTier = 2;

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
	//b->velocity *= 1 + 1.0/64;
	b->velocity += 1.0/64;
	b->r *= 65.0/64;
}

double MegaDeathBulletPower::getDefenseTier(Bullet* b) {
	double value = b->r / Tank::default_radius * destroyWallTier;
	//return (value >= destroyWallTier ? value : 0);
	return value;
}

double MegaDeathBulletPower::getOffenseTier(Bullet* b) {
	double value = b->r / Tank::default_radius * destroyWallTier;
	//return (value >= destroyWallTier ? value : 0); //this is what I originally wanted in JS Tanks, I think, but in practice isn't preferable
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