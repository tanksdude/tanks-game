#include "hidingplaceslevel.h"
#include "constants.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "mylib.h"
#include "resetthings.h"
#include "rng.h"
#include <iostream>

void HidingPlacesLevel::initialize() {
	int randPos = RNG::randFunc() * 5;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40, randPos);

	ColorValueHolder color = ColorValueHolder(0.375f, 0.75f, 0.125f);

	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}

	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+48+60), GAME_HEIGHT/2-(40+20), 60, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, 60, 20, color));
	}
	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+48+60), GAME_HEIGHT/2-(40+20+(80-20)), 60, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, 60, 20, color));
	}
	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+48+60+20), GAME_HEIGHT/2-(40+80), 20, 80);
		WallManager::pushWall(new Wall(pos.x, pos.y, 20, 80, color));
	}

	//traps:
	//PowerupManager::pushPowerup(new PowerSquare(240,160,3));
	//PowerupManager::pushPowerup(new PowerSquare(640-240,160,3));

	PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, 40);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "mines"));
	pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, 40);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "mines"));

	int tempRand = RNG::randFunc() * 2;
	pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-40);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::simplePowerAlternate(0, tempRand, "vanilla-extra", "vanilla"), RandomLevel::simplePowerAlternate(0, tempRand, "shotgun", "multishot")));
	pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-40);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::simplePowerAlternate(1, tempRand, "vanilla-extra", "vanilla"), RandomLevel::simplePowerAlternate(1, tempRand, "shotgun", "multishot")));
	//TODO: better way to randomize power positions when they're in different groups

	tempRand = RNG::randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 60, 80);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, RandomLevel::powerAlternate(i, tempRand, "bounce", "speed"))); //speed=life here
	}
}

Level* HidingPlacesLevel::factory() {
	return new HidingPlacesLevel();
}

HidingPlacesLevel::HidingPlacesLevel() { return; }
