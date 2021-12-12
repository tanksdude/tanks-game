#include "corridorlevel.h"
#include "constants.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "mylib.h"
#include "resetthings.h"
#include "rng.h"

std::unordered_map<std::string, float> CorridorLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}

void CorridorLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40);

	ColorValueHolder color = getDefaultColor();
	int tempRand;
	PositionHolder pos;
	//std::string* paras;

	for (int i = 0; i < 4; i++) {
		//classic JS walls
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}

	pos = RandomLevel::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 70, 20, 140);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, 140, color));
	pos = RandomLevel::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 70, 20, 140);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, 140, color));

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 18, GAME_HEIGHT/2-(36+18), GAME_WIDTH/2-(80+32)-18, 18);
		WallManager::pushWall(new Wall(pos.x, pos.y, GAME_WIDTH/2-(80+32)-18, 18, color));
	}

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "bounce"));

	//traps:
	//for (int i = 0; i < 4; i++) {
	//	pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 140/2-16, 140/2-16);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//}

	tempRand = RNG::randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32)-18, GAME_HEIGHT/2-18);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::powerAlternate(i, tempRand, "invincible", "wallhack")));
	}

	tempRand = RNG::randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32)-18, GAME_HEIGHT/2-(140/2));
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::powerAlternate(i, tempRand, "speed", "big"))); //JS: big=barrier
	}
}

Level* CorridorLevel::factory() {
	return new CorridorLevel();
}

CorridorLevel::CorridorLevel() { return; }
