#include "concealed-powerups-level.h"
#include "constants.h"
#include "level-helper.h"
#include "tank-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "mylib.h"
#include "reset-things.h"
#include "rng.h"

std::unordered_map<std::string, float> ConcealedPowerupsLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 0.25f });
	weights.insert({ "random-vanilla", 0.25f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 0.125f });
	return weights;
}

void ConcealedPowerupsLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40);

	//in JS, power mixing was turned off
	ColorValueHolder color = getDefaultColor();
	int tempRand;
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;

	for (int i = 0; i < 4; i++) {
		//classic JS walls
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}

	pos = LevelHelper::getSymmetricWallPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, 70, 160, 20);
	posArr = new double[4]{ pos.x, pos.y, 160, 20 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "horizontal_lightning")(constructionData));
	delete[] posArr;
	pos = LevelHelper::getSymmetricWallPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, 70, 160, 20);
	posArr = new double[4]{ pos.x, pos.y, 160, 20 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "horizontal_lightning")(constructionData));
	delete[] posArr;

	pos = LevelHelper::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80, 20, GAME_HEIGHT-70*2);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, GAME_HEIGHT-70*2, color));
	pos = LevelHelper::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80, 20, GAME_HEIGHT-70*2);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, GAME_HEIGHT-70*2, color));

	//traps:
	//pos = LevelHelper::getSymmetricPowerupPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20+20);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//pos = LevelHelper::getSymmetricPowerupPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20+20);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "big"));

	tempRand = RNG::randFunc() * 2;
	pos = LevelHelper::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, 20);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, LevelHelper::simplePowerAlternate(0, tempRand, "speed", "invincible")));
	pos = LevelHelper::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, 20);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, LevelHelper::simplePowerAlternate(1, tempRand, "speed", "invincible")));

	tempRand = RNG::randFunc() * 2;
	pos = LevelHelper::getSymmetricPowerupPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 20);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "grenade"));
	pos = LevelHelper::getSymmetricPowerupPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 20);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "grenade"));

	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+20), GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "homing"));
	}
}

Level* ConcealedPowerupsLevel::factory() {
	return new ConcealedPowerupsLevel();
}

ConcealedPowerupsLevel::ConcealedPowerupsLevel() { return; }
