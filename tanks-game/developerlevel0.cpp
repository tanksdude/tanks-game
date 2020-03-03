#pragma once
#include "developerlevel0.h"
#include "constants.h"
#include "tank.h"
#include "powersquare.h"
#include "mylib.h"
#include "randomlevel.h"
#include <math.h>
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "stationaryturret.h" //TODO: vector and hashtable of hazards

void DeveloperLevel0::initialize() {
	int randPos = rand() % 5;
	TankManager::getTank(0)->y = randPos*64 + 32;
	TankManager::getTank(1)->y = (4-randPos)*64 + 32;
	//reset power and shooting

	ColorValueHolder randColor((rand()%256)/255.0, (rand()%256)/255.0, (rand()%256)/255.0);

	for (int i = 0; i < 16; i++) {
		WallManager::pushWall(RandomLevel::makeNewWall(TANK_RADIUS*2.5, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5), GAME_HEIGHT - 2*(TANK_RADIUS*2), randColor));
	}

	HazardManager::pushCircleHazard(new StationaryTurret(GAME_WIDTH/2, GAME_HEIGHT/2, double(rand())/double(RAND_MAX+1) * 2*PI));

	//assumption: TANK_RADIUS=16 (why it would ever be changed is beyond me)
	PowerupManager::pushPowerup(new PowerSquare(20, 20, "speed"));
	PowerupManager::pushPowerup(new PowerSquare(40, 20, "wallhack"));
	PowerupManager::pushPowerup(new PowerSquare(60, 20, "bounce"));
	PowerupManager::pushPowerup(new PowerSquare(80, 20, "multishot"));
	PowerupManager::pushPowerup(new PowerSquare(100, 20, "triple"));
	PowerupManager::pushPowerup(new PowerSquare(120, 20, "homing"));
	PowerupManager::pushPowerup(new PowerSquare(140, 20, "invincible"));
	PowerupManager::pushPowerup(new PowerSquare(160, 20, "grenade"));
	PowerupManager::pushPowerup(new PowerSquare(180, 20, "fire"));
	PowerupManager::pushPowerup(new PowerSquare(200, 20, "blast"));

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2 + 20, "big"));
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2 - 20, "megadeath"));

	std::string* names = new std::string[2]{ "multishot", "multishot" };
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-20, GAME_HEIGHT-20, names, 2));
	names[0] = "speed", names[1] = "wallhack";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-40, GAME_HEIGHT-20, names, 2));
	names[0] = "bounce", names[1] = "homing";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-60, GAME_HEIGHT-20, names, 2));
	names[0] = "bounce", names[1] = "big";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-80, GAME_HEIGHT-20, names, 2));
	names[0] = "wallhack", names[1] = "grenade";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-100, GAME_HEIGHT-20, names, 2));
	delete[] names;
}

DeveloperLevel0::DeveloperLevel0() { return; }
