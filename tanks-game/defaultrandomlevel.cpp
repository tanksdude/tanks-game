#pragma once
#include "defaultrandomlevel.h"
#include "constants.h"
#include "tank.h"
#include "mylib.h"
#include "randomlevel.h"
#include <math.h>
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"

void DefaultRandomLevel::initialize() { //still needs a lot of work
	int randPos = rand() % 5;
	TankManager::getTank(0)->y = randPos*64 + 32;
	TankManager::getTank(1)->y = (4-randPos)*64 + 32;

	ColorValueHolder randColor((rand()%256)/255.0, (rand()%256)/255.0, (rand()%256)/255.0);
	
	for (int i = 0; i < 16; i++) {
		WallManager::pushWall(RandomLevel::makeNewWall(TANK_RADIUS*2.5*2, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5*2), GAME_HEIGHT - 2*(TANK_RADIUS*2), randColor));
	}

	std::string* paras = new std::string[3]{std::to_string(GAME_WIDTH/2), std::to_string(GAME_HEIGHT/2), std::to_string(randFunc() * 2*PI)};
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("stationary turret")(3, paras));
	delete[] paras;

	std::string possiblePowers[] = { "speed", "wallhack", "bounce", "multishot", "triple", "homing", "invincible", "big", "megadeath", "grenade" }; //10
	for (int i = 0; i < 4; i++) {
		int count = rand() % 2 + 1; //{1, 2}
		std::string* randPowers = RandomLevel::getRandomPowers(count, true, 10, possiblePowers);
		PowerupManager::pushPowerup(new PowerSquare(320 - (320-60) * ((i%2)*2-1), 160 - (160-16) * ((i/2)*2-1), randPowers, count));
		delete[] randPowers;
	}
}

DefaultRandomLevel::DefaultRandomLevel() { return; }
