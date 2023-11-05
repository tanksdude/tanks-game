#include "developer-level-4.h"

#include "../constants.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../hazard-manager.h"

ColorValueHolder DeveloperLevel4::getDefaultColor() const {
	//there are no walls in this level...
	return ColorValueHolder(0.25f, 0.25f, 0.25f);
}

std::unordered_map<std::string, float> DeveloperLevel4::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

void DeveloperLevel4::initialize() {
	//primary purpose: an empty-ish level, just to have a lot of free space
	ResetThings::tankPositionReset(ResetThings::default_tankToEdgeDist, GAME_HEIGHT/2);

	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	std::string* names;

	const double tempWidth = 16;
	posArr = new double[3]{ GAME_WIDTH/2 - tempWidth/2, GAME_HEIGHT/2 - tempWidth/2, tempWidth };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushRectHazard("vanilla", "spiral_lava", constructionData);

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
	PowerupManager::pushPowerup(new PowerSquare(240, 20, "big"));
	PowerupManager::pushPowerup(new PowerSquare(260, 20, "megadeath"));

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-20, 20, "vanilla-extra", "mines"));
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-40, 20, "vanilla-extra", "shotgun"));
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-60, 20, "vanilla-extra", "tracking"));
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-80, 20, "vanilla-extra", "barrier"));
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH-100, 20, "vanilla-extra", "shield"));

	PowerupManager::pushPowerup(new PowerSquare(20, GAME_HEIGHT-20, "dev", "inversion"));
	PowerupManager::pushPowerup(new PowerSquare(40, GAME_HEIGHT-20, "dev", "annoying"));
	PowerupManager::pushPowerup(new PowerSquare(60, GAME_HEIGHT-20, "dev", "ultrabounce"));
	PowerupManager::pushPowerup(new PowerSquare(80, GAME_HEIGHT-20, "dev", "wall_sparks"));
	PowerupManager::pushPowerup(new PowerSquare(100, GAME_HEIGHT-20, "dev", "edge_sparks"));
	PowerupManager::pushPowerup(new PowerSquare(120, GAME_HEIGHT-20, "dev", "triple_spread"));
	PowerupManager::pushPowerup(new PowerSquare(140, GAME_HEIGHT-20, "dev", "ring_shooter"));
	PowerupManager::pushPowerup(new PowerSquare(160, GAME_HEIGHT-20, "dev", "banana_split"));

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

Level* DeveloperLevel4::factory() {
	return new DeveloperLevel4();
}

DeveloperLevel4::DeveloperLevel4() { return; }
