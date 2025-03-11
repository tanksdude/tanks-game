#include "dev-movement-restarter-power.h"
#include "../game-manager.h" //settings

std::unordered_map<std::string, float> DevMovementRestarterPower::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

TankPower* DevMovementRestarterPower::makeTankPower() const {
	return new DevMovementRestarterTankPower();
}

BulletPower* DevMovementRestarterPower::makeBulletPower() const {
	return new DevMovementRestarterBulletPower();
}

Power* DevMovementRestarterPower::factory() {
	return new DevMovementRestarterPower();
}

DevMovementRestarterPower::DevMovementRestarterPower() {
	return;
}



BulletPower* DevMovementRestarterTankPower::makeBulletPower() const {
	return new DevMovementRestarterBulletPower();
}

DevMovementRestarterTankPower::DevMovementRestarterTankPower() {
	const GameSettings& game_settings = GameManager::get_settings();
	maxTime = game_settings.PowerupDurationBaseTime;
	timeLeft = game_settings.PowerupDurationBaseTime;
}



InteractionBoolHolder DevMovementRestarterBulletPower::modifiedMovement(Bullet* b) {
	if (!hasRestartedMovement && b->velocity.getMagnitude() <= 0) [[unlikely]] {
		//b->velocity.setMagnitude(b->getInitialVelocity()); //alternate option: reset velocity instead of changing acceleration
		b->acceleration *= -1.25;
		hasRestartedMovement = true; //mainly existed for that alternate option, not really needed anymore
	}
	return { false };
}

TankPower* DevMovementRestarterBulletPower::makeTankPower() const {
	return new DevMovementRestarterTankPower();
}

DevMovementRestarterBulletPower::DevMovementRestarterBulletPower() {
	timeLeft = 0;
	maxTime = -1;

	modifiesMovement = true;

	hasRestartedMovement = false;
}
