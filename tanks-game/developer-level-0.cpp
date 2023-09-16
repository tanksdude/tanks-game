#include "developer-level-0.h"
#include "constants.h"
#include "level-helper.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "reset-things.h"
#include "rng.h"

ColorValueHolder DeveloperLevel0::getDefaultColor() const {
	return currentColor;
}

std::unordered_map<std::string, float> DeveloperLevel0::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

void DeveloperLevel0::initialize() {
	ResetThings::tankPositionReset();

	ColorValueHolder randColor = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	std::string* names;

	for (int i = 0; i < 16; i++) {
		WallManager::pushWall(LevelHelper::makeNewRandomWall(TANK_RADIUS*2.5, TANK_RADIUS*2, GAME_WIDTH - 2*(TANK_RADIUS*2.5), GAME_HEIGHT - 2*(TANK_RADIUS*2), randColor));
	}

	posArr = new double[3]{ GAME_WIDTH/2, GAME_HEIGHT/2, RNG::randFunc() * (2*PI) };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(constructionData));

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
	currentColor = ColorValueHolder(RNG::randFunc(), RNG::randFunc(), RNG::randFunc());
}
