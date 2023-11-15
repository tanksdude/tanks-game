#include "few-obstacles-level.h"

#include "../constants.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"

std::unordered_map<std::string, float> FewObstaclesLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 0.25f });
	weights.insert({ "random-vanilla", 0.25f });
	weights.insert({ "old", 0.5f }); //shared with OldFewObstaclesLevel
	weights.insert({ "random-old", 0.0f }); //OldFewObstaclesLevel used instead
	weights.insert({ "random", 0.125f });
	return weights;
}

void FewObstaclesLevel::initialize() {
	//remember to update OldFewObstaclesLevel
	ResetThings::tankPositionReset();

	ColorValueHolder color = getDefaultColor();
	//PositionHolder pos;
	//GenericFactoryConstructionData constructionData;
	//double* posArr;

	//LevelHelper::pushClassicWalls(color); //JS

	LevelHelper::pushSymmetricWalls_LR(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+40+20), 20, GAME_HEIGHT-128, color);

	LevelHelper::pushSymmetricPowerups_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+40+20/2), (GAME_HEIGHT-128)/2+16, "vanilla", "homing");

	LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+40+20/2)-(16+10), (GAME_HEIGHT-128)/2-20,
		"vanilla", "bounce", "vanilla", "speed"); //JS: speed=tracking
	LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, 40, 40,
		"vanilla", "multishot", "vanilla", "blast");

	//traps:
	//LevelHelper::pushSymmetricPowerups_LR(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+40+20/2)-(16+10), "vanilla", "speed");
	//LevelHelper::pushSymmetricPowerups_UD(GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT-128)/2-20, "vanilla", "speed");

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "vanilla", "banana"));
}

Level* FewObstaclesLevel::factory() {
	return new FewObstaclesLevel();
}

FewObstaclesLevel::FewObstaclesLevel() { return; }
