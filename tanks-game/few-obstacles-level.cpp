#include "few-obstacles-level.h"
#include "constants.h"
#include "random-level.h"
#include "tank-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "mylib.h"
#include "reset-things.h"
#include "rng.h"

std::unordered_map<std::string, float> FewObstaclesLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
	//it's not really an old level (missing the original walls) but whatever
}

void FewObstaclesLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1));

	ColorValueHolder color = getDefaultColor();
	int tempRand;
	PositionHolder pos;
	//GenericFactoryConstructionData constructionData;
	//double* posArr;

	//classic JS walls removed

	pos = RandomLevel::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+40+20), 20, GAME_HEIGHT-128);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, 320-128, color));
	pos = RandomLevel::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+40+20), 20, GAME_HEIGHT-128);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, 320-128, color));

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+40+20/2), (GAME_HEIGHT-128)/2+16);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "homing"));
	}

	tempRand = RNG::randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+40+20/2)-(16+10), (GAME_HEIGHT-128)/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::powerAlternate(i, tempRand, "bounce", "speed"))); //JS: speed=tracking
	}
	tempRand = RNG::randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 40, 40);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::powerAlternate(i, tempRand, "multishot", "blast")));
	}

	//traps:
	//pos = RandomLevel::getSymmetricPowerupPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+40+20/2)-(16+10));
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//pos = RandomLevel::getSymmetricPowerupPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+40+20/2)-(16+10));
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT-128)/2-20);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT-128)/2-20);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "banana"));
}

Level* FewObstaclesLevel::factory() {
	return new FewObstaclesLevel();
}

FewObstaclesLevel::FewObstaclesLevel() { return; }
