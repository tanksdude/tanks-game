#include "homing-power.h"
#include "../game-manager.h" //settings

#include "../constants.h"

const double HomingPower::homingStrength = (2*PI) / 512;

std::unordered_map<std::string, float> HomingPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "supermix", 1.0f });
	weights.insert({ "supermix-vanilla", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

TankPower* HomingPower::makeTankPower() const {
	return new HomingTankPower();
}

BulletPower* HomingPower::makeBulletPower() const {
	return new HomingBulletPower();
}

Power* HomingPower::factory() {
	return new HomingPower();
}

HomingPower::HomingPower() {
	return;
}



BulletPower* HomingTankPower::makeBulletPower() const {
	return new HomingBulletPower();
}

HomingTankPower::HomingTankPower() {
	const GameSettings& game_settings = GameManager::get_settings();
	maxTime = game_settings.PowerupDurationBaseTime;
	timeLeft = game_settings.PowerupDurationBaseTime;
	//in JS, the tank's shooting cooldown was reset
}



#include "../constants.h"

#include "../power-function-helper.h"

InteractionBoolHolder HomingBulletPower::modifiedMovement(Bullet* b) {
	Game_ID targetID = PowerFunctionHelper::homingGenericTarget(b, true);
	if (targetID != -1) {
		PowerFunctionHelper::homingGenericMove(b, targetID, HomingPower::homingStrength);
	} else {
		//do another targeting round, but on hazards/"targetables" instead
		//this will only occur for some sort of "team mode" or single-player campaign
	}
	return { false };
}

InteractionBoolHolder HomingBulletPower::modifiedEdgeCollision(Bullet* b) {
	return { ((b->x + b->r <= 0) || (b->x - b->r >= GAME_WIDTH) ||
	         ((b->velocity.getMagnitude() <= 0) && ((b->y - b->r >= GAME_HEIGHT*2) || (b->y + b->r <= -GAME_HEIGHT)))) };
	//TODO: wanted?
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
