#include "unnamed-level-3.h"

#include "../constants.h"
#include <iostream>

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"
#include "../level-manager.h"

#include "../level-effects/respawning-powerups-level-effect.h"

ColorValueHolder UnnamedLevel3::getDefaultColor() const {
	return ColorValueHolder(1, 1, 1);
}

std::unordered_map<std::string, float> UnnamedLevel3::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.5f });
	weights.insert({ "random-dev", 0.5f });
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

	if (effects.size() < 1) [[unlikely]] {
		throw std::logic_error("ERROR: \"" + getName() + "\" level does not have sufficient level effects!");
	}
	RespawningPowerupsLevelEffect* respawning = static_cast<RespawningPowerupsLevelEffect*>(effects[0]);
	if (respawning->getName() != "respawning_powerups") [[unlikely]] {
		throw std::logic_error("ERROR: \"" + getName() + "\" level does not have \"respawning_powerups\" level effect!");
	}

	posArr = new double[3]{ GAME_WIDTH/2, GAME_HEIGHT/2, -PI/2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard("vanilla", "ginormous_turret", constructionData);

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
