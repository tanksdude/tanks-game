#pragma once
#include "resetthings.h"
#include "constants.h"
#include "bulletmanager.h"

void ResetThings::reset(int) {
	tanks[0]->resetThings(20, GAME_HEIGHT/2, 0, tanks[0]->getID(), tanks[0]->getName());
	tanks[1]->resetThings(GAME_WIDTH - 20, GAME_HEIGHT/2, PI, tanks[1]->getID(), tanks[1]->getName());

	for (int i = 0; i < walls.size(); i++) {
		delete walls[i];
	}
	walls.clear();

	for (int i = 0; i < BulletManager::bullets.size(); i++) {
		delete BulletManager::bullets[i];
	}
	BulletManager::bullets.clear();

	for (int i = 0; i < powerups.size(); i++) {
		delete powerups[i];
	}
	powerups.clear();

	for (int i = 0; i < circleHazards.size(); i++) {
		delete circleHazards[i];
	}
	for (int i = 0; i < rectHazards.size(); i++) {
		delete rectHazards[i];
	}
	circleHazards.clear();
	rectHazards.clear();

	for (int i = 0; i < levels.size(); i++) {
		delete levels[i];
	}
	levels.clear();

	levelLookup["random"]->initialize();
}