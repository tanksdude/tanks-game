#pragma once
#include "developerlevel1.h"
#include "constants.h"
#include <math.h>
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"

void DeveloperLevel1::initialize() {
	TankManager::getTank(0)->y = GAME_HEIGHT/2;
	TankManager::getTank(1)->y = GAME_HEIGHT/2;

	ColorValueHolder wallColor(.25f, .25f, .25f);

	WallManager::pushWall(new Wall(GAME_WIDTH/2 - 80, GAME_HEIGHT/2 - 20, 20, 40, wallColor));
	WallManager::pushWall(new Wall(GAME_WIDTH/2 + 80 - 20, GAME_HEIGHT/2 - 20, 20, 40, wallColor));

	std::string* paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 80 + 20), std::to_string(GAME_HEIGHT/2 - 20), std::to_string((80-20)*2), std::to_string(20*2)};
	//HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("horizontal lightning")(4, paras));
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("lava")(4, paras));
	delete[] paras;

	//assumption: TANK_RADIUS=16 (why it would ever be changed is beyond me)
	PowerupManager::pushPowerup(new PowerSquare(20, 20, "speed"));
	PowerupManager::pushPowerup(new PowerSquare(40, 20, "wallhack"));
	PowerupManager::pushPowerup(new PowerSquare(60, 20, "invincible"));
	PowerupManager::pushPowerup(new PowerSquare(80, 20, "multishot"));
	PowerupManager::pushPowerup(new PowerSquare(100, 20, "big"));
	PowerupManager::pushPowerup(new PowerSquare(120, 20, "homing"));

	std::string* names = new std::string[3]{ "multishot", "multishot", "invincible" };
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-20, GAME_HEIGHT-20, names, 3));
	names[0] = "multishot", names[1] = "big", names[2] = "big";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-40, GAME_HEIGHT-20, names, 3));
	names[0] = "bounce", names[1] = "homing";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-60, GAME_HEIGHT-20, names, 2));
	delete[] names;
}

DeveloperLevel1::DeveloperLevel1() { return; }
