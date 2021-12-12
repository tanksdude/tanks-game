#include "devsymmetrictanksleveleffect.h"
#include "constants.h"
#include "tankmanager.h"

std::unordered_map<std::string, float> DevSymmetricTanksLevelEffect::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.0f });
	weights.insert({ "random-dev", 0.0f });
	//intentional only
	return weights;
}

void DevSymmetricTanksLevelEffect::tick(const Level* parent) {
	TankManager::getTank(1)->x = GAME_WIDTH - TankManager::getTank(0)->x;
	TankManager::getTank(1)->y = GAME_HEIGHT - TankManager::getTank(0)->y;
	if (mirrorAngle) {
		TankManager::getTank(1)->velocity.setAngle(TankManager::getTank(0)->velocity.getAngle() + PI);
	}
}

DevSymmetricTanksLevelEffect::DevSymmetricTanksLevelEffect(bool alsoDoAngle) {
	this->mirrorAngle = alsoDoAngle;
}

DevSymmetricTanksLevelEffect::DevSymmetricTanksLevelEffect() : DevSymmetricTanksLevelEffect(false) {}

LevelEffect* DevSymmetricTanksLevelEffect::factory(int argc, std::string* argv) {
	if (argc >= 1) {
		bool angle = std::stoi(argv[0]);
		return new DevSymmetricTanksLevelEffect(angle);
	}
	return new DevSymmetricTanksLevelEffect();
}
