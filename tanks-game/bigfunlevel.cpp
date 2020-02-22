#pragma once
#include "bigfunlevel.h"
#include "constants.h"
#include "powersquare.h"
#include "wall.h"
#include <math.h>
#include <string>
#include "randomlevel.h"
#include "powerupmanager.h"
#include "wallmanager.h"

void BigFunLevel::initialize() {
	int randPos = rand() % 5;
	tanks[0]->y = randPos * 64 + 32;
	tanks[1]->y = (4 - randPos) * 64 + 32;
	tanks[0]->x = 40;
	tanks[1]->x = GAME_WIDTH - 40;
	//classic

	ColorValueHolder color = ColorValueHolder(.75f, 0.25f, 0.5f);

	for (int i = 0; i < 4; i++) {
		WallManager::pushWall(new Wall(320 - 240*(((3-i)/2) * 2 - 1) - 32*((((3-i)/2) + 1) % 2), i%2 * (320-128), 32, 128, color));
	}

	for (int i = 0; i < 16; i++) {
		WallManager::pushWall(RandomLevel::makeNewWall(80+32+40, 40, GAME_WIDTH - 2*(80+32+40), GAME_HEIGHT - 2*40, color));
	}
	
	std::string possiblePowers[] = { "speed", "invincible", "wallhack", "bounce", "big", "multishot" }; //6
	//include invincible?
	//life and shotgun may have existed around this time but not only are they not in yet I think the craziness would lower
	for (int i = 0; i < 4; i++) {
		int count = rand() % 3 + 1; //{1, 2, 3}
		std::string* randPowers = RandomLevel::getRandomPowers(count, true, 6, possiblePowers);
		PowerupManager::pushPowerup(new PowerSquare(320 - (320-128) * ((i%2)*2-1), 160 - (160-16) * ((i/2)*2-1), randPowers, count));
		delete[] randPowers;
	}
}

BigFunLevel::BigFunLevel() { return; }
