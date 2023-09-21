#include "unnamed-level-3.h"

#include "../constants.h"
#include <iostream>
#include "../rng.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"
#include "../level-manager.h"

#include "../level-effects/respawning-powerups-level-effect.h"

ColorValueHolder UnnamedLevel3::getDefaultColor() const {
	//TODO
	return ColorValueHolder(1, 1, 1);
}

std::unordered_map<std::string, float> UnnamedLevel3::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.0f });
	weights.insert({ "random-dev", 0.0f });
	return weights;
}

void UnnamedLevel3::initialize() {
	//dangerous_center level but with less stuff

	ResetThings::tankPositionReset(ResetThings::default_tankToEdgeDist, GAME_HEIGHT*(3.0/5), 3);

	ColorValueHolder color = getDefaultColor();
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	std::string* types;
	std::string* names;
	//PowerSquare* p;

	LevelEffect* le = nullptr;
	for (int i = 0; i < effects.size(); i++) {
		if (effects[i]->getName() == "respawning_powerups") {
			le = effects[i];
			break;
		}
	}
	if (le == nullptr) {
		throw std::logic_error("ERROR: \"unnamed3\" level does not have \"respawning_powerups\" level effect!");
	}
	RespawningPowerupsLevelEffect* respawning = static_cast<RespawningPowerupsLevelEffect*>(le);
	//TODO: should this be the preferred way of getting specific level effects?

	posArr = new double[3]{ GAME_WIDTH/2, GAME_HEIGHT/2, -PI/2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard("vanilla", "ginormous_turret", constructionData);
	//HazardManager::pushCircleHazard("vanilla", "mother_turret", constructionData);

	LevelHelper::pushSymmetricPowerups_LR(GAME_WIDTH/2, GAME_HEIGHT/2, 64*1.25+20, "vanilla", "invincible");
	respawning->watchLastPowerSquaresPushed(2);

	//starting walls
	LevelHelper::pushSymmetricWalls_LR(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20), 20, GAME_HEIGHT-(40*2), color);

	types = new std::string[2]{ "vanilla", "vanilla" };
	names = new std::string[2]{ "multishot", "homing" };
	LevelHelper::pushSymmetricPowerups_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20)-10, GAME_HEIGHT/2-(40+10), types, names, 2);
	delete[] types; delete[] names;

	LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, 140, 60,
		"vanilla", "bounce", "vanilla", "speed");

	//pocket for a power
	//LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20)-(40), 20, 40, 10, color);

	//pocket powers:
	LevelHelper::pushSymmetricPowerups_LR(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20)-(20)/2, "vanilla", "big");
	respawning->watchLastPowerSquaresPushed(2);
}

Level* UnnamedLevel3::factory() {
	return new UnnamedLevel3();
}

UnnamedLevel3::UnnamedLevel3() {
	bool* temp = new bool[1]{ false };
	GenericFactoryConstructionData constructionData(1, temp);
	effects.push_back(LevelManager::makeLevelEffect("vanilla", "respawning_powerups", constructionData));
}
