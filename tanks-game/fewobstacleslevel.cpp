#include "fewobstacleslevel.h"
#include "constants.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "mylib.h"
#include "resetthings.h"
#include "rng.h"

std::unordered_map<std::string, float> FewObstaclesLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", .5f });
	weights.insert({ "random-old", .5f });
	weights.insert({ "random", .5f });
	return weights;
	//it's not really an old level (missing the original walls) but whatever
}

void FewObstaclesLevel::initialize() {
	int randPos = RNG::randFunc() * 5;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), randPos);

	ColorValueHolder color = getDefaultColor();
	int tempRand;
	PositionHolder pos;
	std::string* paras;

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
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::powerAlternate(i, tempRand, "bounce", "speed"))); //speed=tracking
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
