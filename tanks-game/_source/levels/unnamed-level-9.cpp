#include "unnamed-level-9.h"

#include "../constants.h"
#include <iostream>

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"

ColorValueHolder UnnamedLevel9::getDefaultColor() const {
	return ColorValueHolder(1, 1, 1);
}

std::unordered_map<std::string, float> UnnamedLevel9::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

void UnnamedLevel9::initialize() {
	ResetThings::tankPositionReset(ResetThings::default_tankToEdgeDist, GAME_HEIGHT - 2*20, 2);

	ColorValueHolder color = getDefaultColor();
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	std::string* types;
	std::string* names;

	LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - 80, GAME_HEIGHT/2 - (40+20), 80, 20, color);

	LevelHelper::pushSymmetricWalls_LR(GAME_WIDTH/2, GAME_HEIGHT/2, 40, 20, 180, color);
	LevelHelper::pushSymmetricWalls_LR(GAME_WIDTH/2, GAME_HEIGHT/2, 40+20+40, 20, 180, color);

	LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, 40+20 + 40/2, 180/2 - 20,
		"vanilla-extra", "barrier", "dev", "wall_sparks");

	std::shared_ptr<int[]> patrolCount = std::shared_ptr<int[]>(new int[1]{ 2 }, GenericFactoryConstructionData::deleteArrFunc);
	double* patrolRoute1 = new double[2*2]{
		GAME_WIDTH/2 - 30, GAME_HEIGHT/2 - 10,
		GAME_WIDTH/2 + 30, GAME_HEIGHT/2 - 10
	};
	double* patrolRoute2 = new double[2*2]{
		GAME_WIDTH/2 + 30, GAME_HEIGHT/2 + 10,
		GAME_WIDTH/2 - 30, GAME_HEIGHT/2 + 10
	};
	std::shared_ptr<double[]> patrolWait = std::shared_ptr<double[]>(new double[2]{
		300, 300
	}, GenericFactoryConstructionData::deleteArrFunc);

	posArr = new double[3]{ GAME_WIDTH/2, patrolRoute1[1], PI/2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(2*2, patrolRoute1);
	constructionData.pushData(2, patrolWait);
	HazardManager::pushCircleHazard("vanilla", "patrolling_turret", constructionData);
	posArr = new double[3]{ GAME_WIDTH/2, patrolRoute2[1], -PI/2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(2*2, patrolRoute2);
	constructionData.pushData(2, patrolWait);
	HazardManager::pushCircleHazard("vanilla", "patrolling_turret", constructionData);

	types = new std::string[3]{ "vanilla", "vanilla", "vanilla" };
	//names = new std::string[2]{ "grenade", "homing" };
	names = new std::string[3]{ "blast", "grenade", "speed" };
	LevelHelper::pushSymmetricPowerups_LR(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-20, types, names, 3);
	delete[] types; delete[] names;

	LevelHelper::pushSymmetricPowerups_LR(GAME_WIDTH/2, GAME_HEIGHT/2, 40+20+40+20+10, "vanilla-extra", "shotgun");

	types = new std::string[2]{ "vanilla", "dev" };
	names = new std::string[2]{ "multishot", "inversion" }; //don't add banana
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, types, names, 2));
	delete[] types; delete[] names;

	posArr = new double[3]{ 20, GAME_HEIGHT/2, 16 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard("vanilla", "lightning", constructionData);
	posArr = new double[3]{ GAME_WIDTH - 20, GAME_HEIGHT/2, 16 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard("vanilla", "lightning", constructionData);

	//doesn't feel complete, don't know what to add or change
	//TODO: the answer is probably to not do the different starting position
}

Level* UnnamedLevel9::factory() {
	return new UnnamedLevel9();
}

UnnamedLevel9::UnnamedLevel9() { return; }
