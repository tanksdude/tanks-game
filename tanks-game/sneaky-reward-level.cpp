#include "sneaky-reward-level.h"

#include "constants.h"
#include <stdexcept>
#include <iostream>
#include "rng.h"

#include "reset-things.h"
#include "level-helper.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"

#include "level-manager.h"
#include "respawning-powerups-level-effect.h"

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
	int tempRand;
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	std::string* names;
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
	RespawningPowerupsLevelEffect* respawning = (RespawningPowerupsLevelEffect*)le;
	//TODO: should this be the preferred way of getting specific level effects?

	LevelHelper::pushClassicWalls(color);

	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 20, GAME_HEIGHT/2-(40+10), 10, 40+10);
		WallManager::pushWall(new Wall(pos.x, pos.y, 10, 40+10, color));
	}
	for (int i = 0; i < 2; i++) {
		pos = LevelHelper::getSymmetricWallPositions_UD(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(40+10), 40, 10);
		WallManager::pushWall(new Wall(pos.x, pos.y, 40, 10, color));
	}

	names = new std::string[2]{ "wallhack", "godmode" };
	tempRand = RNG::randFunc() * 2;
	//TOOD: better power alternate functions
	if (tempRand) {
		pos = LevelHelper::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla", names, 2));
		pos = LevelHelper::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "tracking"));
	} else {
		pos = LevelHelper::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "tracking"));
		pos = LevelHelper::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla", names, 2));
	}
	delete[] names;

	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 20+40, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "dev", "annoying"));
	}

	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-32-(20), GAME_HEIGHT/2-(20));
		p = new PowerSquare(pos.x, pos.y, "wallhack");
		PowerupManager::pushPowerup(p);
		respawning->watchPowerSquare(p);
	}
	/*
	pos = LevelHelper::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(40+20+10));
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "wallhack"));
	pos = LevelHelper::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(40+20+10));
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "wallhack"));
	*/

	posArr = new double[4]{ GAME_WIDTH/2-20/2, GAME_HEIGHT/2-80, 20, 80*2 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(constructionData));

	p = new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "dev", "inversion");
	PowerupManager::pushPowerup(p);
	respawning->watchPowerSquare(p);

	//TODO: so technically it's still possible to softlock on this level; should the godmode power be respawning?
}

Level* SneakyRewardLevel::factory() {
	return new SneakyRewardLevel();
}

SneakyRewardLevel::SneakyRewardLevel() {
	bool* temp = new bool[1]{ false };
	GenericFactoryConstructionData constructionData(1, temp);
	effects.push_back(LevelManager::getLevelEffectFactory("vanilla", "respawning_powerups")(constructionData));
}
