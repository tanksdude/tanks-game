#include "megadeathpower.h"

std::unordered_map<std::string, float> MegaDeathPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .25f });
	weights.insert({ "random-vanilla", .25f });
	weights.insert({ "random", .25f });
	return weights;
}

TankPower* MegaDeathPower::makeTankPower() const {
	return new MegaDeathTankPower();
}

BulletPower* MegaDeathPower::makeBulletPower() const {
	return new MegaDeathBulletPower();
}

/*
HazardPower* MegaDeathPower::makeHazardPower() const {
	return new MegaDeathHazardPower();
}
*/

Power* MegaDeathPower::factory() {
	return new MegaDeathPower();
}

MegaDeathPower::MegaDeathPower() {
	return;
}



void MegaDeathTankPower::initialize(Tank* parent) {
	//nothing
}

void MegaDeathTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* MegaDeathTankPower::makeBulletPower() const {
	return new MegaDeathBulletPower();
}

MegaDeathTankPower::MegaDeathTankPower() {
	maxTime = 500;
	timeLeft = 500;
}



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

InteractionBoolHolder MegaDeathBulletPower::modifiedCollisionWithWall(Bullet* b, Wall* w) {
	//tick() happens before this, so modifiesCollisionWithWall will only be set to true if the bullet can destroy walls
	return { false, true };
}

InteractionBoolHolder MegaDeathBulletPower::modifiedMovement(Bullet* b) {
	b->r *= bulletSizeMultiplierPerTick;
	return { false };
}

double MegaDeathBulletPower::getOffenseTier(const Bullet* b) const {
	double value = b->r / (Bullet::default_radius*4) * destroyWallTier;
	//return (value >= destroyWallTier ? floor(value) : 0); //this is what I originally wanted in JS Tanks, I think, but in practice isn't preferable
	return value;
}

double MegaDeathBulletPower::getDefenseTier(const Bullet* b) const {
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

TankPower* MegaDeathBulletPower::makeTankPower() const {
	return new MegaDeathTankPower();
}

MegaDeathBulletPower::MegaDeathBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
	//modifiesCollisionWithWall = true;
	//modifiedCollisionWithWallCanWorkWithOthers = false;
}
