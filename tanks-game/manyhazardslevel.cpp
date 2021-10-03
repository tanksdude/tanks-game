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

	ColorValueHolder color = ColorValueHolder(0xDD/255.0, .5f, .25f);

	PositionHolder* wallArray = new PositionHolder[4];
	for (int i = 0; i < 4; i++) {
		wallArray[i] = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(wallArray[i].x, wallArray[i].y, 32, 128, color));
	}

	std::string* paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 10), std::to_string(128), std::to_string(10*2), std::to_string(GAME_HEIGHT - 128*2)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(4, paras));
	delete[] paras;
	
	const int TURRET_COUNT = 16; //stationary turret count, not regular turret count
	const double turretDistance = (wallArray[3].x - (wallArray[0].x+32)) / (TURRET_COUNT+1);
	for (int i = 0; i < TURRET_COUNT; i++) {
		//bottom half
		std::string* paras = new std::string[3]{std::to_string(wallArray[0].x+32 + turretDistance * (i+1)), std::to_string(16), std::to_string(PI/2)};
		HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(3, paras));
		delete[] paras;
	}
	for (int i = 0; i < TURRET_COUNT; i++) {
		//top half
		std::string* paras = new std::string[3]{std::to_string(wallArray[0].x+32 + turretDistance * (i+1)), std::to_string(GAME_HEIGHT-16), std::to_string(-PI/2)};
		HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(3, paras));
		delete[] paras;
	}
	//^^^ this does not result in the exact placements from JS; in JS, those values were then manually rounded to a nice integer

	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - ((wallArray[0].x+32) + (TANK_RADIUS/2 * 1.5)), 32 + 20 + (TANK_RADIUS/2 * 2.5));
		std::string* paras = new std::string[3]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(PI * (i%2))};
		HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "targeting_turret")(3, paras));
		delete[] paras;
	}

	PositionHolder wallPos1 = RandomLevel::getSymmetricWallPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT - 128*2)/2, 160, 20);
	WallManager::pushWall(new Wall(wallPos1.x, wallPos1.y, 160, 20, color));
	PositionHolder wallPos2 = RandomLevel::getSymmetricWallPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT - 128*2)/2, 160, 20);
	WallManager::pushWall(new Wall(wallPos2.x, wallPos2.y, 160, 20, color));

	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 160/2, (GAME_HEIGHT - 128*2)/2, (GAME_WIDTH/2 - 160/2 - (wallArray[0].x+32)), 20);
		std::string* paras = new std::string[4]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(GAME_WIDTH/2 - 160/2 - (wallArray[0].x+32)), std::to_string(20)};
		HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "horizontal_lightning")(4, paras));
		delete[] paras;
	}

	PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 160/4 * 1.5);
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
		PositionHolder pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 160/4, GAME_HEIGHT/2 - wallPos1.y, 160/4, 50);
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
	//PowerupManager::pushPowerup(new PowerSquare(320, 160-50-40-30, 3));
	//PowerupManager::pushPowerup(new PowerSquare(320, 160+50+40+30, 3));
	//yes, they were evil
}

Level* ManyHazardsLevel::factory() {
	return new ManyHazardsLevel();
}

ManyHazardsLevel::ManyHazardsLevel() { return; }
