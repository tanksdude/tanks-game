#include "default-random-level.h"
#include "constants.h"
#include "mylib.h" //weightedSelect
#include "level-helper.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "reset-things.h"
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
	//return ColorValueHolder(RNG::randFunc(), RNG::randFunc(), RNG::randFunc()); //funky mode
	return currentColor;
}

void DefaultRandomLevel::initialize() { //still needs a lot of work
	ResetThings::tankPositionReset();

	ColorValueHolder randColor = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	//GenericFactoryConstructionData constructionData;
	//double* posArr;

	//some random walls
	for (int i = 0; i < 16; i++) {
		WallManager::pushWall(LevelHelper::makeNewRandomWall(TANK_RADIUS*2.5*2, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5*2), GAME_HEIGHT - 2*(TANK_RADIUS*2), randColor));
	}

	//randomize hazards:
	//get the weight of each hazard
	float* circleHazardWeights = new float[HazardManager::getNumCircleHazardTypes("random-vanilla")];
	float* rectHazardWeights = new float[HazardManager::getNumRectHazardTypes("random-vanilla")];
	for (int i = 0; i < HazardManager::getNumCircleHazardTypes("random-vanilla"); i++) {
		std::string n = HazardManager::getCircleHazardName("random-vanilla", i);
		GenericFactoryConstructionData constructionData;
		CircleHazard* ch = HazardManager::getCircleHazardFactory("random-vanilla", n)(constructionData);
		circleHazardWeights[i] = ch->getWeights()["random-vanilla"];
		delete ch;
	}
	for (int i = 0; i < HazardManager::getNumRectHazardTypes("random-vanilla"); i++) {
		std::string n = HazardManager::getRectHazardName("random-vanilla", i);
		GenericFactoryConstructionData constructionData;
		RectHazard* rh = HazardManager::getRectHazardFactory("random-vanilla", n)(constructionData);
		rectHazardWeights[i] = rh->getWeights()["random-vanilla"];
		delete rh;
	}

	//add the hazards, but randomly based on their weight (if one has a high weight, it can get randomized multiple times)
	float choosingHazardWeights[] = { 0.5f, 1.5f, 2.0f, 1.0f };
	double circleVsRectHazardChoice = double(HazardManager::getNumCircleHazardTypes("random-vanilla"))/double(HazardManager::getNumCircleHazardTypes("random-vanilla") + HazardManager::getNumRectHazardTypes("random-vanilla"));
	int addHazardCount = RNG::randNumInRange(8, 12+1); //v0.2.4 had 5 circle hazards and 5 rect hazards, meaning there used to be ~10 chances to insert hazards
	for (int i = 0; i < addHazardCount; i++) {
		if (RNG::randFunc() < circleVsRectHazardChoice) {
			int count = weightedSelect<float>(choosingHazardWeights, 4); //{0, 1, 2, 3}
			int index = weightedSelect<float>(circleHazardWeights, HazardManager::getNumCircleHazardTypes("random-vanilla")); //randomize which hazard gets chosen (it could be one that was already chosen)
			CircleHazardRandomizationFunction f = HazardManager::getCircleHazardRandomizationFunction("random-vanilla", HazardManager::getCircleHazardName("random-vanilla", index));
			for (int i = 0; i < count; i++) {
				GenericFactoryConstructionData constructionData;
				CircleHazard* ch = f(TANK_RADIUS*2.5*2, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5*2), GAME_HEIGHT - 2*(TANK_RADIUS*2), constructionData);
				if (ch != nullptr) {
					HazardManager::pushCircleHazard(ch);
				}
			}
		} else {
			int count = weightedSelect<float>(choosingHazardWeights, 4); //{0, 1, 2, 3}
			int index = weightedSelect<float>(rectHazardWeights, HazardManager::getNumRectHazardTypes("random-vanilla"));
			RectHazardRandomizationFunction f = HazardManager::getRectHazardRandomizationFunction("random-vanilla", HazardManager::getRectHazardName("random-vanilla", index));
			for (int i = 0; i < count; i++) {
				GenericFactoryConstructionData constructionData;
				RectHazard* rh = f(TANK_RADIUS*2.5*2, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5*2), GAME_HEIGHT - 2*(TANK_RADIUS*2), constructionData);
				if (rh != nullptr) {
					HazardManager::pushRectHazard(rh);
				}
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
		std::string* randPowers = LevelHelper::getRandomPowers(count, "random-vanilla");
		pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-60, GAME_HEIGHT/2-16);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "random-vanilla", randPowers, count));
		delete[] randPowers;
	}

	//throw somethin' special in the center
	std::string* ultimatePowerName = LevelHelper::getRandomPowers(1, "ultimate-vanilla");
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "ultimate-vanilla", ultimatePowerName[0]));
	delete[] ultimatePowerName;
}

Level* DefaultRandomLevel::factory() {
	return new DefaultRandomLevel();
}

DefaultRandomLevel::DefaultRandomLevel() {
	currentColor = ColorValueHolder(RNG::randFunc(), RNG::randFunc(), RNG::randFunc());
}
