#include "defaultrandomlevel.h"
#include "constants.h"
#include "mylib.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "resetthings.h"
#include "rng.h"
#include <iostream>

std::unordered_map<std::string, float> DefaultRandomLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 4.0f });
	weights.insert({ "random-vanilla", 4.0f });
	weights.insert({ "random", 2.0f });
	return weights;
}

ColorValueHolder DefaultRandomLevel::getDefaultColor() const {
	//return ColorValueHolder(RNG::randFunc2(), RNG::randFunc2(), RNG::randFunc2());
	return currentColor;
}

void DefaultRandomLevel::initialize() { //still needs a lot of work
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1));

	ColorValueHolder randColor = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	//std::string* paras;

	//some random walls
	for (int i = 0; i < 16; i++) {
		WallManager::pushWall(RandomLevel::makeNewRandomWall(TANK_RADIUS*2.5*2, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5*2), GAME_HEIGHT - 2*(TANK_RADIUS*2), randColor));
	}

	//randomize hazards:
	//get the weight of each hazard
	float* circleHazardWeights = new float[HazardManager::getNumCircleHazardTypes("random-vanilla")];
	float* rectHazardWeights = new float[HazardManager::getNumRectHazardTypes("random-vanilla")];
	for (int i = 0; i < HazardManager::getNumCircleHazardTypes("random-vanilla"); i++) {
		std::string n = HazardManager::getCircleHazardName("random-vanilla", i);
		CircleHazard* ch = HazardManager::getCircleHazardFactory("random-vanilla", n)(0, nullptr);
		circleHazardWeights[i] = ch->getWeights()["random-vanilla"];
		delete ch;
	}
	for (int i = 0; i < HazardManager::getNumRectHazardTypes("random-vanilla"); i++) {
		std::string n = HazardManager::getRectHazardName("random-vanilla", i);
		RectHazard* rh = HazardManager::getRectHazardFactory("random-vanilla", n)(0, nullptr);
		rectHazardWeights[i] = rh->getWeights()["random-vanilla"];
		delete rh;
	}

	//add the hazards, but randomly based on their weight (if one has a high weight, it can get randomized multiple times)
	float choosingHazardWeights[] = { 0.5f, 1.5f, 2.0f, 1.0f };
	for (int i = 0; i < HazardManager::getNumCircleHazardTypes("random-vanilla"); i++) {
		int count = weightedSelect<float>(choosingHazardWeights, 4); //{0, 1, 2, 3}
		int index = weightedSelect<float>(circleHazardWeights, HazardManager::getNumCircleHazardTypes("random-vanilla")); //randomize which hazard gets chosen (it could be one that was already chosen)
		CircleHazardRandomizationFunction f = HazardManager::getCircleHazardRandomizationFunction("random-vanilla", HazardManager::getCircleHazardName("random-vanilla", index));
		for (int i = 0; i < count; i++) {
			CircleHazard* ch = f(TANK_RADIUS*2.5*2, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5*2), GAME_HEIGHT - 2*(TANK_RADIUS*2), 0, nullptr);
			if (ch != nullptr) {
				HazardManager::pushCircleHazard(ch);
			}
		}
	}
	for (int i = 0; i < HazardManager::getNumRectHazardTypes("random-vanilla"); i++) {
		int count = weightedSelect<float>(choosingHazardWeights, 4); //{0, 1, 2, 3}
		int index = weightedSelect<float>(rectHazardWeights, HazardManager::getNumRectHazardTypes("random-vanilla"));
		RectHazardRandomizationFunction f = HazardManager::getRectHazardRandomizationFunction("random-vanilla", HazardManager::getRectHazardName("random-vanilla", index));
		for (int i = 0; i < count; i++) {
			RectHazard* rh = f(TANK_RADIUS*2.5*2, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5*2), GAME_HEIGHT - 2*(TANK_RADIUS*2), 0, nullptr);
			if (rh != nullptr) {
				HazardManager::pushRectHazard(rh);
			}
		}
	}
	//std::cout << (HazardManager::getNumCircleHazards() + HazardManager::getNumCircleHazards()) << std::endl;

	delete[] circleHazardWeights;
	delete[] rectHazardWeights;

	//randomize powers:
	float choosingPowerWeights[] = { 1.0f, 1.0f, .25f };
	for (int i = 0; i < 4; i++) {
		int count = weightedSelect<float>(choosingPowerWeights, 3) + 1; //{1, 2, 3}
		std::string* randPowers = RandomLevel::getRandomPowers(count, "random-vanilla");
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-60, GAME_HEIGHT/2-16);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "random-vanilla", randPowers, count));
		delete[] randPowers;
	}

	//throw somethin' special in the center
	std::string* ultimatePowerName = RandomLevel::getRandomPowers(1, "ultimate-vanilla");
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "ultimate-vanilla", ultimatePowerName[0]));
	delete[] ultimatePowerName;
}

Level* DefaultRandomLevel::factory() {
	return new DefaultRandomLevel();
}

DefaultRandomLevel::DefaultRandomLevel() {
	//TODO: should this go in the constructor or initialize()?
	currentColor = ColorValueHolder(RNG::randFunc2(), RNG::randFunc2(), RNG::randFunc2());
}
