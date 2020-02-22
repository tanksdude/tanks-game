#pragma once
#include "resetthings.h"
#include "constants.h"
#include "bulletmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"

void ResetThings::reset(int) {
	tanks[0]->resetThings(20, GAME_HEIGHT/2, 0, tanks[0]->getID(), tanks[0]->getName());
	tanks[1]->resetThings(GAME_WIDTH - 20, GAME_HEIGHT/2, PI, tanks[1]->getID(), tanks[1]->getName());

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

	for (int i = 0; i < levels.size(); i++) {
		delete levels[i];
	}
	levels.clear();

	levelLookup["random"]->initialize();
}