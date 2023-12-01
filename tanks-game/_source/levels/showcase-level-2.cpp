#include "showcase-level-2.h"

#include "../constants.h"
#include <algorithm> //std::rotate_copy
#include <iostream>
#include "../rng.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"

ColorValueHolder ShowcaseLevel2::getDefaultColor() const {
	return ColorValueHolder(1, 1, 1);
}

std::unordered_map<std::string, float> ShowcaseLevel2::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.25f });
	weights.insert({ "random-dev", 0.25f });
	return weights;
}

void ShowcaseLevel2::initialize() {
	//based on unnamed3

	ResetThings::tankPositionReset(ResetThings::default_tankToEdgeDist, GAME_HEIGHT*(3.0/5), 3);

	ColorValueHolder color = getDefaultColor();
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	std::string* types;
	std::string* names;
	std::shared_ptr<int[]> patrolCount;

	const double centerOffset = 80;
	patrolCount = std::shared_ptr<int[]>(new int[1]{ 4 }, GenericFactoryConstructionData::deleteArrFunc);
	double* patrolRoute0 = new double[4*2]{
		GAME_WIDTH/2 - centerOffset, GAME_HEIGHT/2 - centerOffset,
		GAME_WIDTH/2 + centerOffset, GAME_HEIGHT/2 - centerOffset,
		GAME_WIDTH/2 + centerOffset, GAME_HEIGHT/2 + centerOffset,
		GAME_WIDTH/2 - centerOffset, GAME_HEIGHT/2 + centerOffset
	};
	double* patrolRoute1 = new double[4*2];
	double* patrolRoute2 = new double[4*2];
	double* patrolRoute3 = new double[4*2];
	std::rotate_copy(patrolRoute0, patrolRoute0+1*2, patrolRoute0+4*2, patrolRoute1);
	std::rotate_copy(patrolRoute0, patrolRoute0+2*2, patrolRoute0+4*2, patrolRoute2);
	std::rotate_copy(patrolRoute0, patrolRoute0+3*2, patrolRoute0+4*2, patrolRoute3);

	posArr = new double[3]{ patrolRoute0[0], patrolRoute0[1], PI/2 * 0 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(4*2, patrolRoute0);
	HazardManager::pushCircleHazard("vanilla", "patrolling_turret", constructionData);

	posArr = new double[3]{ patrolRoute1[0], patrolRoute1[1], PI/2 * 1 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(4*2, patrolRoute1);
	HazardManager::pushCircleHazard("vanilla", "patrolling_turret", constructionData);

	posArr = new double[3]{ patrolRoute2[0], patrolRoute2[1], PI/2 * 2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(4*2, patrolRoute2);
	HazardManager::pushCircleHazard("vanilla", "patrolling_turret", constructionData);

	posArr = new double[3]{ patrolRoute3[0], patrolRoute3[1], PI/2 * 3 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(4*2, patrolRoute3);
	HazardManager::pushCircleHazard("vanilla", "patrolling_turret", constructionData);

	//starting walls
	LevelHelper::pushSymmetricWalls_LR(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20), 20, GAME_HEIGHT-(40*2), color);

	types = new std::string[2]{ "vanilla-extra", "vanilla" };
	names = new std::string[2]{ "barrier", "wallhack" };
	LevelHelper::pushSymmetricPowerups_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20)-10, GAME_HEIGHT/2-(40+10), types, names, 2);
	delete[] types; delete[] names;

	//pocket for a power
	LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20)-(20), 20, 20, 10, color);

	//pocket powers:
	LevelHelper::pushSymmetricPowerups_LR(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20)-(20)/2, "vanilla", "homing");
}

Level* ShowcaseLevel2::factory() {
	return new ShowcaseLevel2();
}

ShowcaseLevel2::ShowcaseLevel2() { return; }
