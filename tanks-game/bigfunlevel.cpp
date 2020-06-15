#include "bigfunlevel.h"
#include "constants.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "mylib.h"
#include "resetthings.h"

std::unordered_map<std::string, double> BigFunLevel::getWeights() {
	std::unordered_map<std::string, double> weights;
	weights.insert({ "vanilla", .5 });
	weights.insert({ "random-vanilla", .5 });
	weights.insert({ "old", .5 });
	weights.insert({ "random-old", .5 });
	weights.insert({ "random", .5 });
	return weights;
}

void BigFunLevel::initialize() {
	int randPos = randFunc() * 5;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40, randPos);
	//classic

	ColorValueHolder color = ColorValueHolder(0.75f, 0.25f, 0.5f);

	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}

	for (int i = 0; i < 16; i++) {
		WallManager::pushWall(RandomLevel::makeNewRandomWall(80+32+40, 40, GAME_WIDTH - 2*(80+32+40), GAME_HEIGHT - 2*40, color));
	}
	
	std::string possiblePowers[] = { "speed", "invincible", "wallhack", "bounce", "big", "multishot" }; //6
	//include invincible?
	//life and shotgun may have existed around this time but not only are they not in yet I think the craziness (fun factor) would lower
	for (int i = 0; i < 4; i++) {
		int count = randFunc()*3 + 1; //{1, 2, 3}
		std::string* randPowers = RandomLevel::getRandomPowers(count, true, 6, possiblePowers);
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(80+32+16), GAME_HEIGHT/2-16);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, randPowers, count));
		delete[] randPowers;
	}
}

Level* BigFunLevel::factory() {
	return new BigFunLevel();
}

BigFunLevel::BigFunLevel() { return; }
