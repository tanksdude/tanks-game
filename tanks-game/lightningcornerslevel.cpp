#include "lightningcornerslevel.h"
#include "constants.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "mylib.h"
#include "resetthings.h"
#include <iostream>

std::unordered_map<std::string, float> LightningCornersLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", .25f });
	weights.insert({ "random-vanilla", .125f });
	weights.insert({ "old", .125f });
	return weights;
}

void LightningCornersLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40);

	ColorValueHolder color = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	std::string* paras;
	std::string* names;

	for (int i = 0; i < 4; i++) {
		//classic JS walls
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80-20, 80-20, 20, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, 20, 20, color));
	}

	//I don't think this should exist
	paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 80 + 20), std::to_string(GAME_HEIGHT/2 - 80 + 20), std::to_string((80-20)*2), std::to_string((80-20)*2)};
	//HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(4, paras));
	delete[] paras;

	paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 80 + 20), std::to_string(GAME_HEIGHT/2 - 80), std::to_string((80-20)*2), std::to_string(20)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "horizontal_lightning")(4, paras));
	delete[] paras;
	paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 80 + 20), std::to_string(GAME_HEIGHT/2 + 80 - 20), std::to_string((80-20)*2), std::to_string(20)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "horizontal_lightning")(4, paras));
	delete[] paras;
	paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 80), std::to_string(GAME_HEIGHT/2 - 80 + 20), std::to_string(20), std::to_string((80-20)*2)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(4, paras));
	delete[] paras;
	paras = new std::string[4]{std::to_string(GAME_WIDTH/2 + 80 - 20), std::to_string(GAME_HEIGHT/2 - 80 + 20), std::to_string(20), std::to_string((80-20)*2)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(4, paras));
	delete[] paras;

	//traps:
	//for (int i = 0; i < 4; i++) {
	//	pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80-20/2, 80+20/2);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//}
	//for (int i = 0; i < 4; i++) {
	//	pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20/2, 80-20/2);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//}

	//unused:
	//for (int i = 0; i < 4; i++) {
	//	pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20/2, 80+20/2);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "banana"));
	//}

	names = new std::string[5]{ "speed", "speed", "speed", "big", "big" };
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, names, 5));
	delete[] names;
	//JS used 4 speed to accomplish the same thing (big bullet speed decrease stacked there) (if using old big power, do what JS does)

	//if the powerup at the center is split, use this:
	//for (int i = 0; i < 4; i++) {
	//	pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 10, 10);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//}

	//not here in the JS level but I feel it should be here:
	pos = RandomLevel::getSymmetricPowerupPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20);
	paras = new std::string[3]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(0)};
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(3, paras));
	delete[] paras;
	pos = RandomLevel::getSymmetricPowerupPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20);
	paras = new std::string[3]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(PI)};
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(3, paras));
	delete[] paras;
	pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20);
	paras = new std::string[3]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(PI/2)};
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(3, paras));
	delete[] paras;
	pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20);
	paras = new std::string[3]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(-PI/2)};
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(3, paras));
	delete[] paras;

	//just trying stuff
	//paras = new std::string[4]{std::to_string(wallArray[0].x+32), std::to_string(0), std::to_string(16), std::to_string(GAME_HEIGHT)};
	//HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(4, paras));
	//delete[] paras;
	//paras = new std::string[4]{std::to_string(wallArray[3].x-16), std::to_string(0), std::to_string(16), std::to_string(GAME_HEIGHT)};
	//HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(4, paras));
	//delete[] paras;
}

Level* LightningCornersLevel::factory() {
	return new LightningCornersLevel();
}

LightningCornersLevel::LightningCornersLevel() { return; }
