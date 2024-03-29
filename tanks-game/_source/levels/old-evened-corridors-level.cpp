#include "old-evened-corridors-level.h"

#include "../constants.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"

std::unordered_map<std::string, float> OldEvenedCorridorsLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "old", 0.5f }); //shared with EvenedCorridorsLevel
	weights.insert({ "random-old", 1.0f });
	return weights;
}

void OldEvenedCorridorsLevel::initialize() {
	ResetThings::tankPositionReset(40);

	ColorValueHolder color = getDefaultColor();
	//PositionHolder pos;
	//GenericFactoryConstructionData constructionData;
	//double* posArr;

	LevelHelper::pushClassicWalls(color);

	LevelHelper::pushSymmetricWalls_LR(GAME_WIDTH/2, GAME_HEIGHT/2, 70, 20, 140, color);

	LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, 18, GAME_HEIGHT/2-(36+18), GAME_WIDTH/2-(80+32)-18, 18, color);

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "vanilla", "bounce"));

	//traps:
	//LevelHelper::pushSymmetricPowerups_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, 140/2-16, 140/2-16, "vanilla", "speed");

	LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32)-18, GAME_HEIGHT/2-18,
		"vanilla", "invincible", "vanilla", "wallhack");

	LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32)-18, GAME_HEIGHT/2-(140/2),
		"vanilla", "speed", "vanilla-extra", "barrier");
}

Level* OldEvenedCorridorsLevel::factory() {
	return new OldEvenedCorridorsLevel();
}

OldEvenedCorridorsLevel::OldEvenedCorridorsLevel() { return; }
