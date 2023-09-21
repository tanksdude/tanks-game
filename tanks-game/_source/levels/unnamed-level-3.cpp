#include "unnamed-level-3.h"

#include "../constants.h"
#include <iostream>
#include "../rng.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"
#include "../level-manager.h"

ColorValueHolder UnnamedLevel3::getDefaultColor() const {
	//TODO
	return ColorValueHolder(1, 1, 1);
}

std::unordered_map<std::string, float> UnnamedLevel3::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.0f });
	weights.insert({ "random-dev", 0.0f });
	return weights;
}

void UnnamedLevel3::initialize() {
	//dangerous_center level but with less stuff

	ResetThings::tankPositionReset(ResetThings::default_tankToEdgeDist, GAME_HEIGHT/2);

	ColorValueHolder color = getDefaultColor();
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;

	posArr = new double[3]{ GAME_WIDTH/2, GAME_HEIGHT/2, -PI/2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard("vanilla", "ginormous_turret", constructionData);
	//HazardManager::pushCircleHazard("vanilla", "mother_turret", constructionData);

	//hiding areas from lone turret level, pushed out
	const double top_gap = TANK_RADIUS*2;
	const double side_length = 60;

	//just use a custom distance
	//LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT/2+40)-(top_gap+side_length)+20, GAME_HEIGHT/2-(top_gap+side_length), side_length-20, 20, color);
	//LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT/2+40)-(top_gap+side_length), GAME_HEIGHT/2-(top_gap+side_length), 20, side_length, color);

	//starting walls
	LevelHelper::pushSymmetricWalls_LR(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20), 20, GAME_HEIGHT-(top_gap+side_length-20)*2, color);

	//pocket for a power
	LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20)-(40), 20, 40, 10, color);
}

Level* UnnamedLevel3::factory() {
	return new UnnamedLevel3();
}

UnnamedLevel3::UnnamedLevel3() { return; }
