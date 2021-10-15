#include "manyhazardslevel.h"
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

std::unordered_map<std::string, float> ManyHazardsLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .25f });
	weights.insert({ "random-vanilla", .25f });
	weights.insert({ "old", .25f });
	weights.insert({ "random-old", .25f });
	weights.insert({ "random", .25f });
	return weights;
}

void ManyHazardsLevel::initialize() {
	int randPos = RNG::randFunc() * 5;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40, randPos);

	//in JS, power mixing was turned off
	ColorValueHolder color = getDefaultColor();
	int tempRand;
	PositionHolder pos;
	std::string* paras;

	PositionHolder* wallArray = new PositionHolder[4];
	for (int i = 0; i < 4; i++) {
		//classic JS walls
		wallArray[i] = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(wallArray[i].x, wallArray[i].y, 32, 128, color));
	}

	paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 10), std::to_string(128), std::to_string(10*2), std::to_string(GAME_HEIGHT - 128*2)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(4, paras));
	delete[] paras;

	const int TURRET_COUNT = 16; //stationary turret count, not regular turret count
	//const double turretDistance = (wallArray[3].x - (wallArray[0].x+32)) / (TURRET_COUNT+1);
	const double turretDistance = (wallArray[3].x - (wallArray[0].x+32) - 8) / (TURRET_COUNT+1);
	for (int i = 0; i < TURRET_COUNT; i++) {
		//bottom half
		//std::string* paras = new std::string[3]{std::to_string(wallArray[0].x+32 + turretDistance * (i+1)), std::to_string(16), std::to_string(PI/2)};
		std::string* paras = new std::string[3]{std::to_string(wallArray[0].x+32+8/2 + turretDistance * (i+1)), std::to_string(16), std::to_string(PI/2)};
		HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(3, paras));
		delete[] paras;
	}
	for (int i = 0; i < TURRET_COUNT; i++) {
		//top half
		//std::string* paras = new std::string[3]{std::to_string(wallArray[0].x+32 + turretDistance * (i+1)), std::to_string(GAME_HEIGHT-16), std::to_string(-PI/2)};
		std::string* paras = new std::string[3]{std::to_string(wallArray[0].x+32+8/2 + turretDistance * (i+1)), std::to_string(GAME_HEIGHT-16), std::to_string(-PI/2)};
		HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(3, paras));
		delete[] paras;
	}
	//JS x positions: [140, 164, ..., 476, 500]; C++ x positions: [136.471, 160.941, ..., 479.059, 503.529]
	//positions have been updated to JS version; old version left commented out

	//this is too much:
	//for (int i = 0; i < TURRET_COUNT-1; i++) {
	//	std::string* paras = new std::string[3]{std::to_string(wallArray[0].x+32+8/2 + turretDistance * (i+1) + 4+8), std::to_string(16), std::to_string(PI/2)};
	//	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "targeting_turret")(3, paras));
	//	delete[] paras;
	//}
	//for (int i = 0; i < TURRET_COUNT-1; i++) {
	//	std::string* paras = new std::string[3]{std::to_string(wallArray[0].x+32+8/2 + turretDistance * (i+1) + 4+8), std::to_string(GAME_HEIGHT-16), std::to_string(-PI/2)};
	//	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "targeting_turret")(3, paras));
	//	delete[] paras;
	//}

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - ((wallArray[0].x+32) + (TANK_RADIUS/2 * 1.5)), 32 + 20 + (TANK_RADIUS/2 * 2.5));
		//pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - ((wallArray[0].x+32) + (TANK_RADIUS/2 * 1.5) + 4), 32 + 20 + (TANK_RADIUS/2 * 2.5));
		std::string* paras = new std::string[3]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(PI * (i%2))};
		HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "targeting_turret")(3, paras));
		delete[] paras;
	}

	//useless traps:
	//for (int i = 0; i < 4; i++) {
	//	pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - (wallArray[0].x+32 + 20), GAME_HEIGHT/2 - 48);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//}

	PositionHolder wallPos1 = RandomLevel::getSymmetricWallPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT - 128*2)/2, 160, 20);
	WallManager::pushWall(new Wall(wallPos1.x, wallPos1.y, 160, 20, color));
	PositionHolder wallPos2 = RandomLevel::getSymmetricWallPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT - 128*2)/2, 160, 20);
	WallManager::pushWall(new Wall(wallPos2.x, wallPos2.y, 160, 20, color));

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 160/2, (GAME_HEIGHT - 128*2)/2, (GAME_WIDTH/2 - 160/2 - (wallArray[0].x+32)), 20);
		std::string* paras = new std::string[4]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(GAME_WIDTH/2 - 160/2 - (wallArray[0].x+32)), std::to_string(20)};
		HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "horizontal_lightning")(4, paras));
		delete[] paras;
	}

	pos = RandomLevel::getSymmetricPowerupPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 160/4 * 1.5);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "barrier")); //replace barrier with speed?
	pos = RandomLevel::getSymmetricPowerupPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 160/4 * 1.5);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "barrier"));

	pos = RandomLevel::getSymmetricWallPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2 - wallPos1.y, 160/2, 50);
	paras = new std::string[4]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(160/2), std::to_string(50)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lava")(4, paras));
	delete[] paras;
	pos = RandomLevel::getSymmetricWallPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2 - wallPos1.y, 160/2, 50);
	paras = new std::string[4]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(160/2), std::to_string(50)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lava")(4, paras));
	delete[] paras;

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 160/4, GAME_HEIGHT/2 - wallPos1.y, 160/4, 50);
		std::string* paras = new std::string[4]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(160/4), std::to_string(50)};
		HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lightning")(4, paras));
		delete[] paras;
	}

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 160/4 * 1.5, (GAME_HEIGHT - 128*2)/2 + 20 + 50 + 21);
		//(160 - 32 - 20 - 50 - 16)/2 = 21
		//(GAME_HEIGHT/2 - (GAME_HEIGHT - 128*2)/2 - [wall height] - [lava height] - [stationary turret distance to edge])/2
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "invincible"));
	}

	//traps:
	//pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT - 128*2)/2 + 20 + 50 + 21);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT - 128*2)/2 + 20 + 50 + 21);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//yes, they were evil

	delete[] wallArray;
}

Level* ManyHazardsLevel::factory() {
	return new ManyHazardsLevel();
}

ManyHazardsLevel::ManyHazardsLevel() { return; }
