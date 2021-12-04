#include "homingpower.h"

std::unordered_map<std::string, float> HomingPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", .5f });
	weights.insert({ "random-old", .5f });
	weights.insert({ "supermix", .5f });
	weights.insert({ "supermix-vanilla", .5f });
	weights.insert({ "random", .5f });
	return weights;
}

TankPower* HomingPower::makeTankPower() const {
	return new HomingTankPower();
}

BulletPower* HomingPower::makeBulletPower() const {
	return new HomingBulletPower();
}

/*
HazardPower* HomingPower::makeHazardPower() const {
	return new HomingHazardPower();
}
*/

Power* HomingPower::factory() {
	return new HomingPower();
}

HomingPower::HomingPower() {
	return;
}



void HomingTankPower::initialize(Tank* parent) {
	//nothing
	//in JS, the tank's shooting cooldown was reset
}

void HomingTankPower::removeEffects(Tank* parent) {
	//nothing
}

BulletPower* HomingTankPower::makeBulletPower() const {
	return new HomingBulletPower();
}

HomingTankPower::HomingTankPower() {
	maxTime = 500;
	timeLeft = 500;
}



#include "powerfunctionhelper.h"
#include "constants.h"

const double HomingBulletPower::homingStrength = 2*PI / 256; //JS Tanks used 2*PI / 512

InteractionBoolHolder HomingBulletPower::modifiedMovement(Bullet* b) {
	Game_ID targetID = PowerFunctionHelper::homingGenericTarget(b, true);
	if (targetID != -1) {
		PowerFunctionHelper::homingGenericMove(b, targetID, HomingBulletPower::homingStrength);
	} else {
		//do another targeting round, but on hazards/"targetables" instead
		//this will only occur for some sort of "team mode" or single-player campaign
	}
	return { false };
}

InteractionBoolHolder HomingBulletPower::modifiedEdgeCollision(Bullet* b) {
	return { ((b->x + b->r <= 0) || (b->x - b->r >= GAME_WIDTH)) }; //TODO: wanted?
}

void HomingBulletPower::initialize(Bullet* parent) {
	//nothing
}

void HomingBulletPower::removeEffects(Bullet* parent) {
	//nothing
}

TankPower* HomingBulletPower::makeTankPower() const {
	return new HomingTankPower();
}

HomingBulletPower::HomingBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;
	modifiesEdgeCollision = true;
}
