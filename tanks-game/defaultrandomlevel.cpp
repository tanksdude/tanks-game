#include "defaultrandomlevel.h"
#include "constants.h"
#include "mylib.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "resetthings.h"

std::unordered_map<std::string, double> DefaultRandomLevel::getWeights() {
	std::unordered_map<std::string, double> weights;
	weights.insert({ "vanilla", 1 });
	weights.insert({ "random", 1 });
	return weights;
}

void DefaultRandomLevel::initialize() { //still needs a lot of work
	int randPos = randFunc() * 5;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), randPos);

	ColorValueHolder randColor(randFunc2(), randFunc2(), randFunc2());
	
	for (int i = 0; i < 16; i++) {
		WallManager::pushWall(RandomLevel::makeNewRandomWall(TANK_RADIUS*2.5*2, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5*2), GAME_HEIGHT - 2*(TANK_RADIUS*2), randColor));
	}

	std::string paras[3] = {std::to_string(GAME_WIDTH/2), std::to_string(GAME_HEIGHT/2), std::to_string(randFunc() * 2*PI)};
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("stationary turret")(3, paras));

	std::string possiblePowers[] = { "speed", "wallhack", "bounce", "multishot", "triple", "homing", "invincible", "big", "megadeath", "grenade" }; //10
	for (int i = 0; i < 4; i++) {
		int count = randFunc()*2 + 1; //{1, 2}
		std::string* randPowers = RandomLevel::getRandomPowers(count, true, 10, possiblePowers);
		PositionHolder pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-60, GAME_HEIGHT/2-16);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, randPowers, count));
		delete[] randPowers;
	}
}

Level* DefaultRandomLevel::factory() {
	return new DefaultRandomLevel();
}

DefaultRandomLevel::DefaultRandomLevel() { return; }
