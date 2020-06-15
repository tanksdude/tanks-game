#include "corridorlevel.h"
#include "constants.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "mylib.h"
#include "resetthings.h"

std::unordered_map<std::string, double> CorridorLevel::getWeights() {
	std::unordered_map<std::string, double> weights;
	weights.insert({ "vanilla", .5 });
	weights.insert({ "random", .5 });
	weights.insert({ "old", .5 });
	weights.insert({ "random-old", .5 });
	return weights;
}

void CorridorLevel::initialize() {
	int randPos = randFunc() * 5;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40, randPos);

	ColorValueHolder color = ColorValueHolder(0x22/255.0, 0.5f, 1.0f);

	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}
	
	PositionHolder pos = RandomLevel::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 70, 20, 140);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, 140, color));
	pos = RandomLevel::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 70, 20, 140);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, 140, color));

	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 18, GAME_HEIGHT/2-(36+18), GAME_WIDTH/2-(80+32)-18, 18);
		WallManager::pushWall(new Wall(pos.x, pos.y, GAME_WIDTH/2-(80+32)-18, 18, color));
	}

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "bounce"));

	int tempRand = randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32)-18, GAME_HEIGHT/2-18);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::powerAlternate(i, tempRand, "invincible", "wallhack")));
	}

	tempRand = randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32)-18, GAME_HEIGHT/2-(140/2));
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::powerAlternate(i, tempRand, "speed", "big"))); //big=life here
	}
}

Level* CorridorLevel::factory() {
	return new CorridorLevel();
}

CorridorLevel::CorridorLevel() { return; }
