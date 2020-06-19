#include "defaultrandomlevel.h"
#include "constants.h"
#include "mylib.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "resetthings.h"

std::unordered_map<std::string, float> DefaultRandomLevel::getWeights() {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 2.0f });
	weights.insert({ "random-vanilla", 2.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

void DefaultRandomLevel::initialize() { //still needs a lot of work
	int randPos = randFunc() * 5;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), randPos);

	ColorValueHolder randColor(randFunc2(), randFunc2(), randFunc2());
	
	for (int i = 0; i < 16; i++) {
		WallManager::pushWall(RandomLevel::makeNewRandomWall(TANK_RADIUS*2.5*2, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5*2), GAME_HEIGHT - 2*(TANK_RADIUS*2), randColor));
	}

	std::string paras[3] = {std::to_string(GAME_WIDTH/2), std::to_string(GAME_HEIGHT/2), std::to_string(randFunc() * 2*PI)};
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("stationary turret")(3, paras));
	
	int powerNum = PowerupManager::getNumPowerTypes("random-vanilla");
	std::string* possiblePowers = new std::string[powerNum];
	for (int i = 0; i < powerNum; i++) {
		possiblePowers[i] = PowerupManager::getPowerName("random-vanilla", i);
	}

	//get stacking status of the powers
	bool* canStack = new bool[powerNum];
	for (int i = 0; i < powerNum; i++) {
		Power* p = PowerupManager::getPowerFactory("random-vanilla", possiblePowers[i])();
		std::vector<std::string> attributes = p->getPowerAttributes();
		canStack[i] = (std::find(attributes.begin(), attributes.end(), "stack") != attributes.end());
		delete p;
	}

	float weights[] = { 1.0f, 1.0f, .25f };
	for (int i = 0; i < 4; i++) {
		int count = weightedSelect<float>(weights, 3) + 1; //{1, 2, 3}
		std::string* randPowers = RandomLevel::getRandomPowers(count, canStack, possiblePowers, powerNum);
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-60, GAME_HEIGHT/2-16);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "random-vanilla", randPowers, count));
		delete[] randPowers;
	}

	delete[] possiblePowers;
	delete[] canStack;
}

Level* DefaultRandomLevel::factory() {
	return new DefaultRandomLevel();
}

DefaultRandomLevel::DefaultRandomLevel() { return; }
