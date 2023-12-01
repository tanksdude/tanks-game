#include "hiding-places-level.h"

#include "../constants.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"

std::unordered_map<std::string, float> HidingPlacesLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 1.0f });
	weights.insert({ "random-vanilla", 0.5f });
	weights.insert({ "old", 0.5f }); //shared with OldHidingPlacesLevel
	weights.insert({ "random-old", 0.25f }); //shared with OldHidingPlacesLevel
	return weights;
}

void HidingPlacesLevel::initialize() {
	//remember to update with OldHidingPlacesLevel
	ResetThings::tankPositionReset(40);

	ColorValueHolder color = getDefaultColor();
	//PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;

	LevelHelper::pushClassicWalls(color);

	LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+48+60), GAME_HEIGHT/2-(40+20), 60, 20, color);
	LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+48+60), GAME_HEIGHT/2-(40+20+(80-20)), 60, 20, color);
	LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+48+60+20), GAME_HEIGHT/2-(40+80), 20, 80, color);

	//traps:
	//LevelHelper::pushSymmetricPowerups_LR(GAME_WIDTH/2, GAME_HEIGHT/2, 80, "vanilla", "speed");

	LevelHelper::pushSymmetricPowerups_UD(GAME_WIDTH/2, GAME_HEIGHT/2, 40, "vanilla-extra", "mines");

	LevelHelper::pushSymmetricPowerups_UD_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-40,
		"vanilla-extra", "shotgun", "vanilla", "multishot");

	LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, 60, 80,
		"vanilla", "bounce", "vanilla", "speed"); //JS: speed=barrier

	//alternate powers:
	//LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, 60, 80,
	//	"vanilla", "multishot", "vanilla-extra", "shotgun");

	//not here in the JS level but I feel it should be here:
	posArr = new double[4]{ GAME_WIDTH/2-20/2, GAME_HEIGHT/2-20/2, 20, 20 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard("vanilla", "no_bullet_zone", constructionData);
}

Level* HidingPlacesLevel::factory() {
	return new HidingPlacesLevel();
}

HidingPlacesLevel::HidingPlacesLevel() { return; }
