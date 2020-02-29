#pragma once
#include "levelmanager.h"

std::vector<Level*> LevelManager::levels;
void LevelManager::initialize() {
	return;
}

Level* const LevelManager::getLevel(int index) {
	return levels[index];
}

LevelEffect* const LevelManager::getLevelEffect(int level_index, int index) {
	return levels[level_index]->effects[index];
}

void LevelManager::pushLevel(Level* l) {
	levels.push_back(l);
}

/*
int LevelManager::getNumLevelEffects() {
	int total = 0;
	for (int i = 0; i < levels.size(); i++) {
		total += LevelManager::getLevel_numEffects(i);
	}
	return total;
}
*/

void LevelManager::clearLevels() {
	for (int i = 0; i < levels.size(); i++) {
		delete levels[i];
	}
	levels.clear();
}

/*
void LevelManager::deleteLevel(int index) {
	delete levels[index];
	levels.erase(levels.begin() + index);
}
*/
