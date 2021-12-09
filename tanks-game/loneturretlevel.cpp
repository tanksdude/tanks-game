#include "loneturretlevel.h"
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
	std::string* paras;

	//TODO: should these walls exist?
	//for (int i = 0; i < 4; i++) {
	//	//classic JS walls
	//	pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
	//	WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	//}

	paras = new std::string[3]{std::to_string(GAME_WIDTH/2), std::to_string(GAME_HEIGHT/2), std::to_string(PI/2 * (floor(RNG::randFunc()*2)*2-1))};
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "targeting_turret")(3, paras));
	delete[] paras;

	//rectangular lightning surrounding turret:
	//paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 80), std::to_string(GAME_HEIGHT/2 - 40), std::to_string(80*2), std::to_string(40*2)};
	//HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lightning")(4, paras));
	//delete[] paras;
	//paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 40), std::to_string(GAME_HEIGHT/2 - 40), std::to_string(40*2), std::to_string(40*2)};
	//HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lightning")(4, paras));
	//delete[] paras;

	pos = RandomLevel::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80, 20, 80);
	paras = new std::string[4]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(20), std::to_string(80)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(4, paras));
	delete[] paras;
	pos = RandomLevel::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80, 20, 80);
	paras = new std::string[4]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(20), std::to_string(80)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(4, paras));
	delete[] paras;

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80, 40, 20, 80);
		WallManager::pushWall(new Wall(pos.x, pos.y, 20, 80, color));
	}
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20, 40, 80-20, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, 80-20, 20, color));
	}

	//outer powers:
	tempRand = RNG::randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20+(80-20)/2, 40+20+(80-20)/2);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", RandomLevel::powerAlternate(i, tempRand, "barrier", "tracking")));
	}

	//inner powers:
	tempRand = RNG::randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80/2, 40+20+(80-20)/2);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla", RandomLevel::powerAlternate(i, tempRand, "grenade", "blast")));
	}

	//alternate inner powers:
	//tempRand = RNG::randFunc() * 2;
	//pos = RandomLevel::getSymmetricPowerupPositions_DiagForwardSlash(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80/2, 40+20+(80-20)/2);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::simplePowerAlternate(0, tempRand, "fire", "grenade")));
	//pos = RandomLevel::getSymmetricPowerupPositions_DiagForwardSlash(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80/2, 40+20+(80-20)/2);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::simplePowerAlternate(1, tempRand, "fire", "grenade")));
	//tempRand = RNG::randFunc() * 2;
	//pos = RandomLevel::getSymmetricPowerupPositions_DiagBackwardSlash(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80/2, 40+20+(80-20)/2);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::simplePowerAlternate(0, tempRand, "bounce", "blast")));
	//pos = RandomLevel::getSymmetricPowerupPositions_DiagBackwardSlash(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80/2, 40+20+(80-20)/2);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::simplePowerAlternate(1, tempRand, "bounce", "blast")));
}

Level* LoneTurretLevel::factory() {
	return new LoneTurretLevel();
}

LoneTurretLevel::LoneTurretLevel() { return; }
