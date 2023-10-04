#include "evened-corridors-level.h"

#include "../constants.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"

std::unordered_map<std::string, float> EvenedCorridorsLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 0.5f });
	weights.insert({ "random-vanilla", 0.5f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 0.25f });
	return weights;
}

void EvenedCorridorsLevel::initialize() {
	ResetThings::tankPositionReset(40);

	ColorValueHolder color = getDefaultColor();
	//PositionHolder pos;
	//GenericFactoryConstructionData constructionData;
	//double* posArr;

	LevelHelper::pushClassicWalls(color);

	LevelHelper::pushSymmetricWalls_LR(GAME_WIDTH/2, GAME_HEIGHT/2, 70, 20, 140, color);

	LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, 18, GAME_HEIGHT/2-(36+18), GAME_WIDTH/2-(80+32)-18, 18, color);

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "bounce"));

	//traps:
	//LevelHelper::pushSymmetricPowerups_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, 140/2-16, 140/2-16, "vanilla", "speed");

	LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32)-18, GAME_HEIGHT/2-18,
		"vanilla", "invincible", "vanilla", "wallhack");

	LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32)-18, GAME_HEIGHT/2-(140/2),
		"vanilla", "speed", "vanilla", "big"); //JS: big=barrier
}

Level* EvenedCorridorsLevel::factory() {
	return new EvenedCorridorsLevel();
}

EvenedCorridorsLevel::EvenedCorridorsLevel() { return; }
