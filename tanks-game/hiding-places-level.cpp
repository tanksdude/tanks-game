#include "hiding-places-level.h"

#include "constants.h"
#include "rng.h"

#include "reset-things.h"
#include "level-helper.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"

std::unordered_map<std::string, float> HidingPlacesLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 1.0f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", .5f });
	return weights;
}

void HidingPlacesLevel::initialize() {
	ResetThings::tankPositionReset(40);

	ColorValueHolder color = getDefaultColor();
	int tempRand;
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;

	LevelHelper::pushClassicWalls(color);

	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+48+60), GAME_HEIGHT/2-(40+20), 60, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, 60, 20, color));
	}
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+48+60), GAME_HEIGHT/2-(40+20+(80-20)), 60, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, 60, 20, color));
	}
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+48+60+20), GAME_HEIGHT/2-(40+80), 20, 80);
		WallManager::pushWall(new Wall(pos.x, pos.y, 20, 80, color));
	}

	//traps:
	//pos = LevelHelper::getSymmetricPowerupPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//pos = LevelHelper::getSymmetricPowerupPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));

	pos = LevelHelper::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, 40);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "mines"));
	pos = LevelHelper::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, 40);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "mines"));

	tempRand = RNG::randFunc() * 2;
	pos = LevelHelper::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-40);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, LevelHelper::simplePowerAlternate(0, tempRand, "vanilla-extra", "vanilla"), LevelHelper::simplePowerAlternate(0, tempRand, "shotgun", "multishot")));
	pos = LevelHelper::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-40);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, LevelHelper::simplePowerAlternate(1, tempRand, "vanilla-extra", "vanilla"), LevelHelper::simplePowerAlternate(1, tempRand, "shotgun", "multishot")));
	//TODO: better way to randomize power positions when they're in different groups

	tempRand = RNG::randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 60, 80);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, LevelHelper::powerAlternate(i, tempRand, "bounce", "speed"))); //JS: speed=barrier
	}

	//alternate powers:
	//tempRand = RNG::randFunc() * 2;
	//for (int i = 0; i < 4; i++) {
	//	pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 60, 80);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::powerAlternate(i, tempRand, "vanilla", "vanilla-extra"), LevelHelper::powerAlternate(i, tempRand, "multishot", "shotgun")));
	//}

	//not here in the JS level but I feel it should be here:
	posArr = new double[4]{ GAME_WIDTH/2-20/2, GAME_HEIGHT/2-20/2, 20, 20 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(constructionData));
}

Level* HidingPlacesLevel::factory() {
	return new HidingPlacesLevel();
}

HidingPlacesLevel::HidingPlacesLevel() { return; }
