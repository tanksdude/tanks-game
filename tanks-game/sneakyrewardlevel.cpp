#include "sneakyrewardlevel.h"
#include "constants.h"
#include "levelmanager.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "mylib.h"
#include "resetthings.h"
#include "rng.h"
#include <iostream>
#include <stdexcept>
#include "respawningpowerupsleveleffect.h" //I hope this isn't permanent...

ColorValueHolder SneakyRewardLevel::getDefaultColor() const {
	return ColorValueHolder(0x99/255.0, 0x66/255.0, 0xCC/255.0); //amethyst
	//should lavender be used? (#E6E6FA)
}

std::unordered_map<std::string, float> SneakyRewardLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", .5f });
	weights.insert({ "random-vanilla", .125f }); //should this even appear?
	return weights;
}

void SneakyRewardLevel::initialize() {
	int randPos = RNG::randFunc() * 5;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40, randPos);

	ColorValueHolder color = getDefaultColor();
	int tempRand;
	PositionHolder pos;
	std::string* paras;
	PowerSquare* p;

	LevelEffect* le = nullptr;
	for (int i = 0; i < effects.size(); i++) {
		if (effects[i]->getName() == "respawning_powerups") {
			le = effects[i];
		}
	}
	if (le == nullptr) {
		throw std::logic_error("ERROR: \"sneaky_reward\" level does not have \"respawning_powerups\" level effect!");
	}
	RespawningPowerupsLevelEffect* respawning = (RespawningPowerupsLevelEffect*)le;
	//TODO: should this be the preferred way of getting specific level effects?

	for (int i = 0; i < 4; i++) {
		//classic JS walls
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 20, GAME_HEIGHT/2-(40+10), 10, 40+10);
		WallManager::pushWall(new Wall(pos.x, pos.y, 10, 40+10, color));
	}
	for (int i = 0; i < 2; i++) {
		pos = RandomLevel::getSymmetricWallPositions_UD(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(40+10), 40, 10);
		WallManager::pushWall(new Wall(pos.x, pos.y, 40, 10, color));
	}

	std::string* names = new std::string[2]{ "wallhack", "godmode" };
	tempRand = RNG::randFunc() * 2;
	//TOOD: better power alternate functions
	if (tempRand) {
		pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla", names, 2));
		pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "tracking"));
	} else {
		pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "tracking"));
		pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla", names, 2));
	}
	delete[] names;

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 20+40, GAME_HEIGHT/2-20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "dev", "annoying"));
	}

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-32-(20), GAME_HEIGHT/2-(20));
		p = new PowerSquare(pos.x, pos.y, "wallhack");
		PowerupManager::pushPowerup(p);
		respawning->watchPowerSquare(p);
	}
	/*
	pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(40+20+10));
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "wallhack"));
	pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(40+20+10));
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "wallhack"));
	*/

	paras = new std::string[4]{std::to_string(GAME_WIDTH/2-20/2), std::to_string(GAME_HEIGHT/2-80), std::to_string(20), std::to_string(80*2)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(4, paras));
	delete[] paras;

	p = new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "dev", "inversion");
	PowerupManager::pushPowerup(p);
	respawning->watchPowerSquare(p);
}

Level* SneakyRewardLevel::factory() {
	return new SneakyRewardLevel();
}

SneakyRewardLevel::SneakyRewardLevel() {
	effects.push_back(LevelManager::getLevelEffectFactory("vanilla", "respawning_powerups")(0, nullptr));
}
