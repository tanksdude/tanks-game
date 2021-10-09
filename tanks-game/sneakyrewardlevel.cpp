#include "sneakyrewardlevel.h"
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

void SneakyRewardLevel::initialize() {
	int randPos = RNG::randFunc() * 5;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40, randPos);

	ColorValueHolder color = ColorValueHolder(0x99/255.0, 0x66/255.0, 0xCC/255.0); //amethyst
	//should lavender be used? (#E6E6FA)

	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}

	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 20, GAME_HEIGHT/2-(40+10), 10, 40+10);
		WallManager::pushWall(new Wall(pos.x, pos.y, 10, 40+10, color));
	}
	for (int i = 0; i < 2; i++) {
		PositionHolder pos = RandomLevel::getSymmetricWallPositions_UD(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(40+10), 40, 10);
		WallManager::pushWall(new Wall(pos.x, pos.y, 40, 10, color));
	}

	std::string* names = new std::string[2]{ "wallhack", "godmode" };
	int tempRand = RNG::randFunc() * 2;
	//TOOD: better power alternate functions
	if (tempRand) {
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla", names, 2));
		pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "tracking"));
	} else {
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "tracking"));
		pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla", names, 2));
	}
	delete[] names;

	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 20+40, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "dev", "annoying"));
	}

	//TODO: infinitely respawning powers
	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-32-(20), GAME_HEIGHT/2-(20));
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "wallhack"));
	}
	/*
	pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(40+20+10));
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "wallhack"));
	pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(40+20+10));
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "wallhack"));
	*/
	
	std::string* paras = new std::string[4]{std::to_string(GAME_WIDTH/2-20/2), std::to_string(GAME_HEIGHT/2-80), std::to_string(20), std::to_string(80*2)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(4, paras));
	delete[] paras;
}

Level* SneakyRewardLevel::factory() {
	return new SneakyRewardLevel();
}

SneakyRewardLevel::SneakyRewardLevel() { return; }
