#include "megadeathbulletpower.h"
#include "megadeathtankpower.h"
#include "constants.h"

const double MegaDeathBulletPower::destroyWallTier = DESTRUCTION_TIER;
const double MegaDeathBulletPower::bulletSizeMultiplierPerTick = 65.0/64.0;

void MegaDeathBulletPower::tick(Bullet* b) {
	if (getOffenseTier(b) >= destroyWallTier) {
		modifiesCollisionWithWall = true;
		modifiedCollisionWithWallCanWorkWithOthers = false;
	} else { //the size could have decreased
		modifiesCollisionWithWall = false;
		modifiedCollisionWithWallCanWorkWithOthers = true;
	}
}

PowerInteractionBoolHolder MegaDeathBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	//tick() happens before this, so modifiesCollisionWithWall will only be set to true if the bullet can destroy walls
	return { false, true };
}

PowerInteractionBoolHolder MegaDeathBulletPower::modifiedMovement(Bullet* b) {
	b->r *= bulletSizeMultiplierPerTick;
	return { false };
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

void MegaDeathBulletPower::initialize(Bullet* parent) {
	//nothing
}

void MegaDeathBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* MegaDeathBulletPower::makeTankPower() {
	return new MegaDeathTankPower();
}

MegaDeathBulletPower::MegaDeathBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
	//modifiesCollisionWithWall = true;
	//modifiedCollisionWithWallCanWorkWithOthers = false;
}
