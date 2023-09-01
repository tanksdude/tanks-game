#include "lightning-corners-level.h"
#include "constants.h"
#include "level-helper.h"
#include "tank-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "reset-things.h"
#include <iostream>

std::unordered_map<std::string, float> LightningCornersLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", .5f });
	//weights.insert({ "random-vanilla", .25f });
	weights.insert({ "old", .25f });
	//weights.insert({ "random-old", .25f });
	return weights;
}

void LightningCornersLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40);

	ColorValueHolder color = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	std::string* names;

	for (int i = 0; i < 4; i++) {
		//classic JS walls
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}

	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80-20, 80-20, 20, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, 20, 20, color));
	}

	//I don't think this should exist
	posArr = new double[4]{ GAME_WIDTH/2 - 80 + 20, GAME_HEIGHT/2 - 80 + 20, (80-20)*2, (80-20)*2 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	//HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(constructionData));
	delete[] posArr;

	posArr = new double[4]{ GAME_WIDTH/2 - 80 + 20, GAME_HEIGHT/2 - 80, (80-20)*2, 20 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "horizontal_lightning")(constructionData));
	delete[] posArr;
	posArr = new double[4]{ GAME_WIDTH/2 - 80 + 20, GAME_HEIGHT/2 + 80 - 20, (80-20)*2, 20 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "horizontal_lightning")(constructionData));
	delete[] posArr;
	posArr = new double[4]{ GAME_WIDTH/2 - 80, GAME_HEIGHT/2 - 80 + 20, 20, (80-20)*2 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(constructionData));
	delete[] posArr;
	posArr = new double[4]{ GAME_WIDTH/2 + 80 - 20, GAME_HEIGHT/2 - 80 + 20, 20, (80-20)*2 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(constructionData));
	delete[] posArr;

	//traps:
	//for (int i = 0; i < 4; i++) {
	//	pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80-20/2, 80+20/2);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//}
	//for (int i = 0; i < 4; i++) {
	//	pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20/2, 80-20/2);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//}

	//unused:
	//for (int i = 0; i < 4; i++) {
	//	pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20/2, 80+20/2);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "banana"));
	//}

	names = new std::string[5]{ "speed", "speed", "speed", "big", "big" };
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, names, 5));
	delete[] names;
	//JS used 4 speed to accomplish the same thing (big bullet speed decrease stacked there) (if using old big power, do what JS does)

	//if the powerup at the center is split, use this:
	//for (int i = 0; i < 4; i++) {
	//	pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 10, 10);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//}

	//not here in the JS level but I feel it should be here:
	pos = LevelHelper::getSymmetricPowerupPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20);
	posArr = new double[3]{ pos.x, pos.y, 0 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(constructionData));
	delete[] posArr;
	pos = LevelHelper::getSymmetricPowerupPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20);
	posArr = new double[3]{ pos.x, pos.y, PI };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(constructionData));
	delete[] posArr;
	pos = LevelHelper::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20);
	posArr = new double[3]{ pos.x, pos.y, PI/2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(constructionData));
	delete[] posArr;
	pos = LevelHelper::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20);
	posArr = new double[3]{ pos.x, pos.y, -PI/2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(constructionData));
	delete[] posArr;

	//just trying stuff
	//posArr = new double[4]{ wallArray[0].x+32, 0, 16, GAME_HEIGHT };
	//constructionData = GenericFactoryConstructionData(4, posArr);
	//HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(constructionData));
	//delete[] posArr;
	//posArr = new double[4]{ wallArray[3].x-16, 0, 16, GAME_HEIGHT };
	//constructionData = GenericFactoryConstructionData(4, posArr);
	//HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(constructionData));
	//delete[] posArr;
}

Level* LightningCornersLevel::factory() {
	return new LightningCornersLevel();
}

LightningCornersLevel::LightningCornersLevel() { return; }
