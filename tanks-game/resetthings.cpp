#pragma once
#include "resetthings.h"
#include "constants.h"
#include "tankmanager.h"
#include "bulletmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "levelmanager.h"
#include "hazardmanager.h"

void ResetThings::reset(int) {
	TankManager::tanks[0]->resetThings(20, GAME_HEIGHT/2, 0, TankManager::tanks[0]->getID(), TankManager::tanks[0]->getName());
	TankManager::tanks[1]->resetThings(GAME_WIDTH - 20, GAME_HEIGHT/2, PI, TankManager::tanks[1]->getID(), TankManager::tanks[1]->getName());

	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		delete WallManager::walls[i];
	}
	WallManager::walls.clear();

	for (int i = 0; i < BulletManager::bullets.size(); i++) {
		delete BulletManager::bullets[i];
	}
	BulletManager::bullets.clear();

	for (int i = 0; i < PowerupManager::getNumPowerups(); i++) {
		delete PowerupManager::powerups[i];
	}
	PowerupManager::powerups.clear();

	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		delete HazardManager::circleHazards[i];
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		delete HazardManager::rectHazards[i];
	}
	HazardManager::circleHazards.clear();
	HazardManager::rectHazards.clear();

	//below code is just LevelManager::clearLevels(), but it's not being called in case it needs to be modified later
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		delete LevelManager::levels[i];
	}
	LevelManager::levels.clear();

	int randLevel = rand() % LevelManager::getNumLevelTypes();
	std::string levelName = LevelManager::getLevelName(randLevel);
	if (levelName != "default random" || levelName == "dev0" || levelName == "empty") {
		int randLevel = rand() % LevelManager::getNumLevelTypes();
		std::string levelName = LevelManager::getLevelName(randLevel);
		if (levelName == "empty") {
			int randLevel = rand() % LevelManager::getNumLevelTypes();
			std::string levelName = LevelManager::getLevelName(randLevel);
		}
	}
	LevelManager::getLevelByName(levelName)->initialize();
}