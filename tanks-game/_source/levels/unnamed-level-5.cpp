#include "unnamed-level-5.h"

#include "../constants.h"
#include <iostream>
#include "../rng.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"

ColorValueHolder UnnamedLevel5::getDefaultColor() const {
	return ColorValueHolder(1, 1, 1);
}

std::unordered_map<std::string, float> UnnamedLevel5::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.0f });
	weights.insert({ "random-dev", 0.0f });
	return weights;
}

void UnnamedLevel5::initialize() {
	ResetThings::tankPositionReset();

	ColorValueHolder color = getDefaultColor();
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	//std::string* types;
	//std::string* names;

	//simply a test level showing how the old ginormous turret would have worked (its old behavior was so lame it had to be changed)

	posArr = new double[3]{ GAME_WIDTH/2, GAME_HEIGHT/2, 0 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard("vanilla", "ginormous_turret", constructionData);

	LevelHelper::pushSymmetricPowerups_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, 192, 96, "vanilla", "invincible");

	LevelHelper::pushSymmetricWalls_UD(GAME_WIDTH/2 - 192, GAME_HEIGHT/2 + 96, 10, 40, 10, color);
	LevelHelper::pushSymmetricWalls_LR(GAME_WIDTH/2 - 192, GAME_HEIGHT/2 + 96, 10, 10, 20, color);

	LevelHelper::pushSymmetricWalls_UD(GAME_WIDTH/2 - 192, GAME_HEIGHT/2 - 96, 10, 40, 10, color);
	LevelHelper::pushSymmetricWalls_LR(GAME_WIDTH/2 - 192, GAME_HEIGHT/2 - 96, 10, 10, 20, color);

	LevelHelper::pushSymmetricWalls_UD(GAME_WIDTH/2 + 192, GAME_HEIGHT/2 + 96, 10, 40, 10, color);
	LevelHelper::pushSymmetricWalls_LR(GAME_WIDTH/2 + 192, GAME_HEIGHT/2 + 96, 10, 10, 20, color);

	LevelHelper::pushSymmetricWalls_UD(GAME_WIDTH/2 + 192, GAME_HEIGHT/2 - 96, 10, 40, 10, color);
	LevelHelper::pushSymmetricWalls_LR(GAME_WIDTH/2 + 192, GAME_HEIGHT/2 - 96, 10, 10, 20, color);

	pos = LevelHelper::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40, 20, 20);
	posArr = new double[4]{ pos.x, pos.y, 20, 20 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard("vanilla", "no_bullet_zone", constructionData);
	pos = LevelHelper::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40, 20, 20);
	posArr = new double[4]{ pos.x, pos.y, 20, 20 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard("vanilla", "no_bullet_zone", constructionData);
}

Level* UnnamedLevel5::factory() {
	return new UnnamedLevel5();
}

UnnamedLevel5::UnnamedLevel5() { return; }
