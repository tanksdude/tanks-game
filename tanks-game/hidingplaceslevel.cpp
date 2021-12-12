#include "hidingplaceslevel.h"
#include "constants.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "mylib.h"
#include "resetthings.h"
#include "rng.h"
#include <iostream>

std::unordered_map<std::string, float> HidingPlacesLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 1.0f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", 1.0f });
	weights.insert({ "random-old", .5f });
	return weights;
}

void HidingPlacesLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40);

	ColorValueHolder color = getDefaultColor();
	int tempRand;
	PositionHolder pos;
	std::string* paras;

	for (int i = 0; i < 4; i++) {
		//classic JS walls
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+48+60), GAME_HEIGHT/2-(40+20), 60, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, 60, 20, color));
	}
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+48+60), GAME_HEIGHT/2-(40+20+(80-20)), 60, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, 60, 20, color));
	}
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+48+60+20), GAME_HEIGHT/2-(40+80), 20, 80);
		WallManager::pushWall(new Wall(pos.x, pos.y, 20, 80, color));
	}

	//traps:
	//pos = RandomLevel::getSymmetricPowerupPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//pos = RandomLevel::getSymmetricPowerupPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));

	pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, 40);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "mines"));
	pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, 40);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "mines"));

	tempRand = RNG::randFunc() * 2;
	pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-40);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::simplePowerAlternate(0, tempRand, "vanilla-extra", "vanilla"), RandomLevel::simplePowerAlternate(0, tempRand, "shotgun", "multishot")));
	pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-40);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::simplePowerAlternate(1, tempRand, "vanilla-extra", "vanilla"), RandomLevel::simplePowerAlternate(1, tempRand, "shotgun", "multishot")));
	//TODO: better way to randomize power positions when they're in different groups

	tempRand = RNG::randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 60, 80);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::powerAlternate(i, tempRand, "bounce", "speed"))); //JS: speed=barrier
	}

	//alternate powers:
	//tempRand = RNG::randFunc() * 2;
	//for (int i = 0; i < 4; i++) {
	//	pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 60, 80);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::powerAlternate(i, tempRand, "vanilla", "vanilla-extra"), RandomLevel::powerAlternate(i, tempRand, "multishot", "shotgun")));
	//}

	//not here in the JS level but I feel it should be here:
	paras = new std::string[4]{std::to_string(GAME_WIDTH/2-20/2), std::to_string(GAME_HEIGHT/2-20/2), std::to_string(20), std::to_string(20)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(4, paras));
	delete[] paras;
}

Level* HidingPlacesLevel::factory() {
	return new HidingPlacesLevel();
}

HidingPlacesLevel::HidingPlacesLevel() { return; }
