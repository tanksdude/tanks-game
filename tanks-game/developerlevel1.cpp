#include "developerlevel1.h"
#include "constants.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "randomlevel.h"
#include "resetthings.h"
//#include <iostream>

ColorValueHolder DeveloperLevel1::getDefaultColor() const {
	return ColorValueHolder(0.25f, 0.25f, 0.25f);
}

std::unordered_map<std::string, float> DeveloperLevel1::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", .5f });
	weights.insert({ "random-dev", .5f });
	return weights;
}

void DeveloperLevel1::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), ResetThings::default_tankToEdgeDist, GAME_HEIGHT/2);

	ColorValueHolder wallColor = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	std::string* paras;
	std::string* names;

	PositionHolder wallPos1 = RandomLevel::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 60, 20, 80);
	WallManager::pushWall(new Wall(wallPos1.x, wallPos1.y, 20, 80, wallColor));
	PositionHolder wallPos2 = RandomLevel::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 60, 20, 80);
	WallManager::pushWall(new Wall(wallPos2.x, wallPos2.y, 20, 80, wallColor));
	WallManager::pushWall(new Wall(wallPos2.x, GAME_HEIGHT-20, 20, 20, wallColor));

	paras = new std::string[4]{std::to_string(wallPos2.x), std::to_string(wallPos2.y+80), std::to_string(20), std::to_string((GAME_HEIGHT-20)-(wallPos2.y+80))};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(4, paras));
	delete[] paras;
	paras = new std::string[4]{std::to_string(wallPos1.x), std::to_string(wallPos1.y+80), std::to_string(20), std::to_string(GAME_HEIGHT-(wallPos1.y+80))};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(4, paras));
	delete[] paras;
	paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 80 + 20), std::to_string(GAME_HEIGHT/2 - 40), std::to_string(60*2), std::to_string(20*2 - 10)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "horizontal_lightning")(4, paras));
	delete[] paras;
	paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 80 + 20), std::to_string(GAME_HEIGHT/2 - 120), std::to_string(30*2), std::to_string(30*2)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lightning")(4, paras));
	delete[] paras;
	paras = new std::string[3]{std::to_string(GAME_WIDTH/2 + 30), std::to_string(GAME_HEIGHT/2 - 90), std::to_string(30)};
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "lightning")(3, paras));
	delete[] paras;
	paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 80 + 20), std::to_string(GAME_HEIGHT/2 + 10), std::to_string(60*2), std::to_string(20*2 - 10)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lava")(4, paras));
	delete[] paras;
	paras = new std::string[3]{std::to_string(GAME_WIDTH/2), std::to_string(GAME_HEIGHT/2 + 100), std::to_string(40)};
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "lava")(3, paras));
	delete[] paras;
	paras = new std::string[3]{std::to_string(GAME_WIDTH/2), std::to_string(GAME_HEIGHT/2), std::to_string(PI/2)};
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "targeting_turret")(3, paras));
	delete[] paras;

	//assumption: TANK_RADIUS=16 (why it would ever be changed is beyond me)
	PowerupManager::pushPowerup(new PowerSquare(20, 20, "speed"));
	PowerupManager::pushPowerup(new PowerSquare(40, 20, "wallhack"));
	PowerupManager::pushPowerup(new PowerSquare(60, 20, "invincible"));
	PowerupManager::pushPowerup(new PowerSquare(80, 20, "multishot"));
	PowerupManager::pushPowerup(new PowerSquare(100, 20, "big"));
	PowerupManager::pushPowerup(new PowerSquare(120, 20, "homing"));
	PowerupManager::pushPowerup(new PowerSquare(140, 20, "grenade"));
	PowerupManager::pushPowerup(new PowerSquare(160, 20, "banana"));

	names = new std::string[3]{ "multishot", "multishot", "invincible" };
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-20, GAME_HEIGHT-20, names, 3));
	names[0] = "multishot", names[1] = "big", names[2] = "big";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-40, GAME_HEIGHT-20, names, 3));
	names[0] = "bounce", names[1] = "homing";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-60, GAME_HEIGHT-20, names, 2));
	delete[] names;

	/*
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		std::cout << "wall" << i << ": " << WallManager::getWall(i)->x << ", " << WallManager::getWall(i)->y << ", " << WallManager::getWall(i)->w << ", " << WallManager::getWall(i)->h << std::endl;
	}
	for (int i = 0; i < PowerupManager::getNumPowerups(); i++) {
		std::cout << "powerup" << i << ": " << PowerupManager::getPowerup(i)->x << ", " << PowerupManager::getPowerup(i)->y << std::endl;
	}
	*/
}

Level* DeveloperLevel1::factory() {
	return new DeveloperLevel1();
}

DeveloperLevel1::DeveloperLevel1() { return; }
