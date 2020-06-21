#include "concealedpowerupslevel.h"
#include "constants.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "mylib.h"
#include "resetthings.h"

std::unordered_map<std::string, float> ConcealedPowerupsLevel::getWeights() {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", .5f });
	weights.insert({ "random-old", .5f });
	weights.insert({ "random", .5f });
	return weights;
}

void ConcealedPowerupsLevel::initialize() {
	int randPos = randFunc() * 5;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40, randPos);

	ColorValueHolder color = ColorValueHolder(0xAA/255.0, 0xBB/255.0, 0x99/255.0);

	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}
	
	PositionHolder pos = RandomLevel::getSymmetricWallPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, 90, 160, 20);
	std::string* paras = new std::string[4]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(160), std::to_string(20)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "horizontal lightning")(4, paras));
	delete[] paras;
	pos = RandomLevel::getSymmetricWallPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, 90, 160, 20);
	paras = new std::string[4]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(160), std::to_string(20)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "horizontal lightning")(4, paras));
	delete[] paras;

	pos = RandomLevel::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80, 20, GAME_HEIGHT-70*2);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, GAME_HEIGHT-70*2, color));
	pos = RandomLevel::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80, 20, GAME_HEIGHT-70*2);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, GAME_HEIGHT-70*2, color));

	//traps:
	//PowerupManager::pushPowerup(new PowerSquare(320-100-20,160,3));
	//PowerupManager::pushPowerup(new PowerSquare(320+80+20+20,160,3));

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "big"));
	int tempRand = randFunc() * 2;
	pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, 20);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::simplePowerAlternate(0, tempRand, "speed", "invincible")));
	pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, 20);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::simplePowerAlternate(1, tempRand, "speed", "invincible")));

	tempRand = randFunc() * 2;
	pos = RandomLevel::getSymmetricPowerupPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 20);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "grenade"));
	pos = RandomLevel::getSymmetricPowerupPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 20);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "grenade"));
	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+20), GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "homing"));
	}
}

Level* ConcealedPowerupsLevel::factory() {
	return new ConcealedPowerupsLevel();
}

ConcealedPowerupsLevel::ConcealedPowerupsLevel() { return; }
