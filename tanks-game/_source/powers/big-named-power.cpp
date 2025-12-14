#include "big-named-power.h"
#include "../game-manager.h" //settings

std::unordered_map<std::string, float> BigNamedPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

TankPower* BigNamedPower::makeTankPower() const {
	return new BigNamedTankPower();
}

BulletPower* BigNamedPower::makeBulletPower() const {
	return new BigNamedBulletPower();
}

Power* BigNamedPower::factory() {
	return new BigNamedPower();
}

BigNamedPower::BigNamedPower() {
	return;
}



BulletPower* BigNamedTankPower::makeBulletPower() const {
	return new BigNamedBulletPower();
}

BigNamedTankPower::BigNamedTankPower() {
	const GameSettings& game_settings = GameManager::get_settings();
	maxTime = game_settings.PowerupDurationBaseTime;
	timeLeft = game_settings.PowerupDurationBaseTime;
}



InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> BigNamedBulletPower::modifiedCollisionWithWall(const Bullet* b, const Wall* w) {
	return { false, true, nullptr, nullptr };
}

TankPower* BigNamedBulletPower::makeTankPower() const {
	return new BigNamedTankPower();
}

BigNamedBulletPower::BigNamedBulletPower() {
	modifiesCollisionWithWall = true;
	modifiedCollisionWithWallCanWorkWithOthers = false;

	bulletRadiusStacks = true;
}
