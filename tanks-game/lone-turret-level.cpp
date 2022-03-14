#include "lone-turret-level.h"
#include "constants.h"
#include "level-helper.h"
#include "tank-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "mylib.h"
#include "reset-things.h"
#include "rng.h"
#include <iostream>

std::unordered_map<std::string, float> LoneTurretLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 1.0f });
	weights.insert({ "random-vanilla", .5f }); //adjust later
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", .5f });
	return weights;
}

void LoneTurretLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40);

	ColorValueHolder color = getDefaultColor();
	int tempRand;
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;

	//TODO: should these walls exist?
	//for (int i = 0; i < 4; i++) {
	//	//classic JS walls
	//	pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
	//	WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	//}

	posArr = new double[3]{ GAME_WIDTH/2, GAME_HEIGHT/2, PI/2 * (floor(RNG::randFunc()*2)*2-1) };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "targeting_turret")(constructionData));
	delete[] posArr;

	//rectangular lightning surrounding turret:
	//posArr = new double[4]{ GAME_WIDTH/2 - 80, GAME_HEIGHT/2 - 40, 80*2, 40*2 };
	//constructionData = GenericFactoryConstructionData(4, posArr);
	//HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lightning")(constructionData));
	//delete[] posArr;
	//posArr = new double[4]{ GAME_WIDTH/2 - 40, GAME_HEIGHT/2 - 40, 40*2, 40*2 };
	//constructionData = GenericFactoryConstructionData(4, posArr);
	//HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lightning")(constructionData));
	//delete[] posArr;

	pos = LevelHelper::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80, 20, 80);
	posArr = new double[4]{ pos.x, pos.y, 20, 80 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(constructionData));
	delete[] posArr;
	pos = LevelHelper::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80, 20, 80);
	posArr = new double[4]{ pos.x, pos.y, 20, 80 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(constructionData));
	delete[] posArr;

	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80, 40, 20, 80);
		WallManager::pushWall(new Wall(pos.x, pos.y, 20, 80, color));
	}
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20, 40, 80-20, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, 80-20, 20, color));
	}

	//outer powers:
	tempRand = RNG::randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20+(80-20)/2, 40+20+(80-20)/2);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", LevelHelper::powerAlternate(i, tempRand, "barrier", "tracking")));
	}

	//inner powers:
	tempRand = RNG::randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80/2, 40+20+(80-20)/2);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla", LevelHelper::powerAlternate(i, tempRand, "grenade", "blast")));
	}

	//alternate inner powers:
	//tempRand = RNG::randFunc() * 2;
	//pos = LevelHelper::getSymmetricPowerupPositions_DiagForwardSlash(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80/2, 40+20+(80-20)/2);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, LevelHelper::simplePowerAlternate(0, tempRand, "fire", "grenade")));
	//pos = LevelHelper::getSymmetricPowerupPositions_DiagForwardSlash(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80/2, 40+20+(80-20)/2);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, LevelHelper::simplePowerAlternate(1, tempRand, "fire", "grenade")));
	//tempRand = RNG::randFunc() * 2;
	//pos = LevelHelper::getSymmetricPowerupPositions_DiagBackwardSlash(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80/2, 40+20+(80-20)/2);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, LevelHelper::simplePowerAlternate(0, tempRand, "bounce", "blast")));
	//pos = LevelHelper::getSymmetricPowerupPositions_DiagBackwardSlash(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80/2, 40+20+(80-20)/2);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, LevelHelper::simplePowerAlternate(1, tempRand, "bounce", "blast")));
}

Level* LoneTurretLevel::factory() {
	return new LoneTurretLevel();
}

LoneTurretLevel::LoneTurretLevel() { return; }
