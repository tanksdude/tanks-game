#include "sneaky-reward-level.h"

#include "../constants.h"
#include <stdexcept>
#include <iostream>

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"
#include "../level-manager.h"

#include "../level-effects/respawning-powerups-level-effect.h"

ColorValueHolder SneakyRewardLevel::getDefaultColor() const {
	return ColorValueHolder(0x99/255.0, 0x66/255.0, 0xCC/255.0); //amethyst
	//should lavender be used? (#E6E6FA)
}

std::unordered_map<std::string, float> SneakyRewardLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", .5f });
	weights.insert({ "random-vanilla", .25f }); //should this even appear?
	return weights;
}

void SneakyRewardLevel::initialize() {
	ResetThings::tankPositionReset(40);

	ColorValueHolder color = getDefaultColor();
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	std::string *types1, *types2;
	std::string *names1, *names2;
	PowerSquare* p;

	LevelEffect* le = nullptr;
	for (int i = 0; i < effects.size(); i++) {
		if (effects[i]->getName() == "respawning_powerups") {
			le = effects[i];
			break;
		}
	}
	if (le == nullptr) {
		throw std::logic_error("ERROR: \"sneaky_reward\" level does not have \"respawning_powerups\" level effect!");
	}
	RespawningPowerupsLevelEffect* respawning = static_cast<RespawningPowerupsLevelEffect*>(le);
	//TODO: should this be the preferred way of getting specific level effects?

	LevelHelper::pushClassicWalls(color);

	LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, 20, GAME_HEIGHT/2-(40+10), 10, 40+10, color);
	LevelHelper::pushSymmetricWalls_UD(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(40+10), 40, 10, color);

	types1 = new std::string[2]{ "vanilla", "vanilla" };
	names1 = new std::string[2]{ "wallhack", "godmode" };
	types2 = new std::string[1]{ "vanilla-extra" };
	names2 = new std::string[1]{ "tracking" }; //maybe it would be less confusing to switch the numbers on these arrays
	LevelHelper::pushSymmetricPowerups_UD_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-20,
		types1, names1, 2, types2, names2, 1);
	delete[] types1; delete[] names1; delete[] types2; delete[] names2;
	//softlock solution:
	respawning->watchLastPowerSquaresPushed(2, 1000);

	LevelHelper::pushSymmetricPowerups_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, 20+40, GAME_HEIGHT/2-20, "dev", "annoying");
	//respawning->watchLastPowerSquaresPushed(4);

	LevelHelper::pushSymmetricPowerups_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, 240-32-(20), GAME_HEIGHT/2-(20), "vanilla", "wallhack");
	respawning->watchLastPowerSquaresPushed(4);
	/*
	LevelHelper::pushSymmetricPowerups_UD(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(40+20+10), "vanilla", "wallhack");
	*/

	posArr = new double[4]{ GAME_WIDTH/2-20/2, GAME_HEIGHT/2-80, 20, 80*2 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard("vanilla", "no_bullet_zone", constructionData);

	p = new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "dev", "inversion");
	PowerupManager::pushPowerup(p);
	respawning->watchPowerSquare(p);
}

Level* SneakyRewardLevel::factory() {
	return new SneakyRewardLevel();
}

SneakyRewardLevel::SneakyRewardLevel() {
	bool* temp = new bool[1]{ false };
	GenericFactoryConstructionData constructionData(1, temp);
	effects.push_back(LevelManager::makeLevelEffect("vanilla", "respawning_powerups", constructionData));
}
