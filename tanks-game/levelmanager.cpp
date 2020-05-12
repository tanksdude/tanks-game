#pragma once
#include "levelmanager.h"

std::vector<Level*> LevelManager::levels;
std::unordered_map<std::string, Level*> LevelManager::levelLookup;
std::unordered_map<std::string, Level*> LevelManager::devLevelLookup;
std::vector<Level*> LevelManager::levelList;
std::vector<std::string> LevelManager::levelNameList;

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


void LevelManager::addLevelToHashmap(Level* l) {
	levelNameList.push_back(l->getName());
	levelList.push_back(l);
	levelLookup.insert({ l->getName(), l });
}

void LevelManager::addDevLevelToHashmap(Level* l) {
	devLevelLookup.insert({ l->getName(), l });
}

Level* LevelManager::getLevelByName(std::string name) {
	return levelLookup[name];
}

Level* LevelManager::getDevLevelByName(std::string name) {
	return devLevelLookup[name];
}

std::string LevelManager::getLevelName(int index) {
	return levelNameList[index];
}

int LevelManager::getNumLevelTypes() {
	return levelNameList.size();
}