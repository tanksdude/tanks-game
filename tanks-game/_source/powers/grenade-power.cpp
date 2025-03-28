#include "grenade-power.h"
#include "../game-manager.h" //settings

const float  GrenadePower::degradeAmount = .875; //JS: .75
const double GrenadePower::growAmount = 65/64.0;

std::unordered_map<std::string, float> GrenadePower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 0.5f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 0.5f });
	weights.insert({ "supermix", 1.0f });
	weights.insert({ "supermix-vanilla", 1.0f });
	weights.insert({ "random", 0.5f });
	return weights;
}

TankPower* GrenadePower::makeTankPower() const {
	return new GrenadeTankPower();
}

BulletPower* GrenadePower::makeBulletPower() const {
	return new GrenadeBulletPower();
}

Power* GrenadePower::factory() {
	return new GrenadePower();
}

GrenadePower::GrenadePower() {
	return;
}



BulletPower* GrenadeTankPower::makeBulletPower() const {
	return new GrenadeBulletPower();
}

GrenadeTankPower::GrenadeTankPower() {
	const GameSettings& game_settings = GameManager::get_settings();
	maxTime = game_settings.PowerupDurationBaseTime;
	timeLeft = game_settings.PowerupDurationBaseTime;
	//JS: maxTime = 1000
	//in JS, the tank's shooting cooldown was reset
}



InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> GrenadeBulletPower::modifiedCollisionWithWall(const Bullet* b, const Wall* w) {
	if (b->velocity.getMagnitude() <= 0) {
		return { b->isDead(), false, new BulletUpdateStruct(0,0,0,0,0, -GrenadePower::degradeAmount), nullptr };
	} else {
		if (b->acceleration < 0) {
			return { false, false, new BulletUpdateStruct(0,0,0, b->acceleration, 0,0), nullptr };
		}
		return { false, false, nullptr, nullptr };
	}
}

TankPower* GrenadeBulletPower::makeTankPower() const {
	return new GrenadeTankPower();
}

GrenadeBulletPower::GrenadeBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesCollisionWithWall = true;

	bulletRadiusGrowMultiplies_Stationary = true;
}
