#include "defaultrandomlevel.h"
#include "constants.h"
#include "mylib.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "resetthings.h"
#include <iostream>

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
	
	//some random walls
	for (int i = 0; i < 16; i++) {
		WallManager::pushWall(RandomLevel::makeNewRandomWall(TANK_RADIUS*2.5*2, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5*2), GAME_HEIGHT - 2*(TANK_RADIUS*2), randColor));
	}

	//randomize hazards:
	float choosingHazardWeights[] = { 0.5f, 1.5f, 2.0f, 1.0f };
	for (int i = 0; i < HazardManager::getNumCircleHazardTypes(); i++) {
		int count = weightedSelect<float>(choosingHazardWeights, 4); //{0, 1, 2, 3}
		CircleHazardRandomizationFunction f = HazardManager::getCircleHazardRandomizationFunction(HazardManager::getCircleHazardName(i));
		for (int i = 0; i < count; i++) {
			CircleHazard* ch = f(TANK_RADIUS*2.5*2, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5*2), GAME_HEIGHT - 2*(TANK_RADIUS*2), 0, nullptr);
			if (ch != nullptr) {
				HazardManager::pushCircleHazard(ch);
			}
		}
	}
	for (int i = 0; i < HazardManager::getNumRectHazardTypes(); i++) {
		int count = weightedSelect<float>(choosingHazardWeights, 4); //{0, 1, 2, 3}
		RectHazardRandomizationFunction f = HazardManager::getRectHazardRandomizationFunction(HazardManager::getRectHazardName(i));
		for (int i = 0; i < count; i++) {
			RectHazard* rh = f(TANK_RADIUS*2.5*2, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5*2), GAME_HEIGHT - 2*(TANK_RADIUS*2), 0, nullptr);
			if (rh != nullptr) {
				HazardManager::pushRectHazard(rh);
			}
		}
	}
	//std::cout << (HazardManager::getNumCircleHazards() + HazardManager::getNumCircleHazards()) << std::endl;
	
	//randomize powers:
	int powerNum = PowerupManager::getNumPowerTypes("random-vanilla");
	std::string* possiblePowers = new std::string[powerNum];
	for (int i = 0; i < powerNum; i++) {
		possiblePowers[i] = PowerupManager::getPowerName("random-vanilla", i);
	}

	//get stacking status and weight of the powers
	bool* canStack = new bool[powerNum];
	float* powerWeights = new float[powerNum];
	for (int i = 0; i < powerNum; i++) {
		Power* p = PowerupManager::getPowerFactory("random-vanilla", possiblePowers[i])();
		std::vector<std::string> attributes = p->getPowerAttributes();
		canStack[i] = (std::find(attributes.begin(), attributes.end(), "stack") != attributes.end());
		powerWeights[i] = p->getWeights()["random-vanilla"];
		delete p;
	}

	float choosingPowerWeights[] = { 1.0f, 1.0f, .25f };
	for (int i = 0; i < 4; i++) {
		int count = weightedSelect<float>(choosingPowerWeights, 3) + 1; //{1, 2, 3}
		std::string* randPowers = RandomLevel::getRandomPowers(count, canStack, possiblePowers, powerWeights, powerNum);
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-60, GAME_HEIGHT/2-16);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "random-vanilla", randPowers, count));
		delete[] randPowers;
	}

	delete[] possiblePowers;
	delete[] canStack;
	delete[] powerWeights;
}

Level* DefaultRandomLevel::factory() {
	return new DefaultRandomLevel();
}

DefaultRandomLevel::DefaultRandomLevel() { return; }
