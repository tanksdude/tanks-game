#include "developer-level-3.h"
#include "constants.h"
#include "random-level.h"
#include "tank-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "reset-things.h"

ColorValueHolder DeveloperLevel3::getDefaultColor() const {
	return ColorValueHolder(0.25f, 0.25f, 0.25f);
}

std::unordered_map<std::string, float> DeveloperLevel3::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.5f });
	weights.insert({ "random-dev", 0.5f });
	return weights;
}

void DeveloperLevel3::initialize() {
	//primary purpose: stupid powerup showcase
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), ResetThings::default_tankToEdgeDist, GAME_HEIGHT/2);

	ColorValueHolder wallColor = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	std::string* names;

	//from dev1
	pos = RandomLevel::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 60, 20, 80);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, 80, wallColor));
	pos = RandomLevel::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 60, 20, 80);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, 80, wallColor));

	posArr = new double[4]{ GAME_WIDTH/2 - 80 + 20, GAME_HEIGHT/2 + 10, 60*2, 20*2 - 10 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lava")(constructionData));
	delete[] posArr;
	posArr = new double[3]{ GAME_WIDTH/2, GAME_HEIGHT/2 + 100, 40 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "lightning")(constructionData));
	delete[] posArr;
	posArr = new double[3]{ GAME_WIDTH/2, GAME_HEIGHT/2, PI/2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(constructionData));
	delete[] posArr;

	//from dev2
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
	PowerupManager::pushPowerup(new PowerSquare(240, 20, "big"));
	PowerupManager::pushPowerup(new PowerSquare(260, 20, "megadeath"));

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-20, 20, "vanilla-extra", "mines"));
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-40, 20, "vanilla-extra", "shotgun"));
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-60, 20, "vanilla-extra", "tracking"));
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-80, 20, "vanilla-extra", "barrier"));
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-100, 20, "vanilla-extra", "shield"));

	//PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "godmode"));

	PowerupManager::pushPowerup(new PowerSquare(20, GAME_HEIGHT-20, "speed"));
	PowerupManager::pushPowerup(new PowerSquare(40, GAME_HEIGHT-20, "speed"));
	PowerupManager::pushPowerup(new PowerSquare(60, GAME_HEIGHT-20, "speed"));
	PowerupManager::pushPowerup(new PowerSquare(100, GAME_HEIGHT-20, "dev", "inversion"));
	PowerupManager::pushPowerup(new PowerSquare(120, GAME_HEIGHT-20, "dev", "annoying"));
	PowerupManager::pushPowerup(new PowerSquare(140, GAME_HEIGHT-20, "dev", "other_stuff_is_poison"));

	names = new std::string[2]{ "multishot", "multishot" };
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-20, GAME_HEIGHT-20, names, 2));
	names[0] = "bounce", names[1] = "homing";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-60, GAME_HEIGHT-20, names, 2));
	names[0] = "banana", names[1] = "banana";
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-120, GAME_HEIGHT-20, names, 2));
	delete[] names;
}

Level* DeveloperLevel3::factory() {
	return new DeveloperLevel3();
}

DeveloperLevel3::DeveloperLevel3() { return; }
