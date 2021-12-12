#include "developerlevel0.h"
#include "constants.h"
#include "mylib.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "resetthings.h"
#include "rng.h"

ColorValueHolder DeveloperLevel0::getDefaultColor() const {
	//return ColorValueHolder(RNG::randFunc2(), RNG::randFunc2(), RNG::randFunc2());
	return currentColor;
}

std::unordered_map<std::string, float> DeveloperLevel0::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

void DeveloperLevel0::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1));

	ColorValueHolder randColor = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	std::string* paras;
	std::string* names;

	for (int i = 0; i < 16; i++) {
		WallManager::pushWall(RandomLevel::makeNewRandomWall(TANK_RADIUS*2.5, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5), GAME_HEIGHT - 2*(TANK_RADIUS*2), randColor));
	}

	paras = new std::string[3]{std::to_string(GAME_WIDTH/2), std::to_string(GAME_HEIGHT/2), std::to_string(RNG::randFunc() * 2*PI)};
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(3, paras));
	delete[] paras;

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
	PowerupManager::pushPowerup(new PowerSquare(220, 20, "banana"));

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2 + 20, "big"));
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2 - 20, "megadeath"));

	names = new std::string[2]{ "multishot", "multishot" };
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-20, GAME_HEIGHT-20, names, 2));
	names[0] = "speed", names[1] = "wallhack";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-40, GAME_HEIGHT-20, names, 2));
	names[0] = "bounce", names[1] = "homing";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-60, GAME_HEIGHT-20, names, 2));
	names[0] = "bounce", names[1] = "big";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-80, GAME_HEIGHT-20, names, 2));
	names[0] = "wallhack", names[1] = "grenade";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-100, GAME_HEIGHT-20, names, 2));
	names[0] = "banana", names[1] = "banana";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-120, GAME_HEIGHT-20, names, 2));
	delete[] names;
}

Level* DeveloperLevel0::factory() {
	return new DeveloperLevel0();
}

DeveloperLevel0::DeveloperLevel0() {
	//TODO: should this go in the constructor or initialize()?
	currentColor = ColorValueHolder(RNG::randFunc2(), RNG::randFunc2(), RNG::randFunc2());
}
