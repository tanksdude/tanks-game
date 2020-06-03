#pragma once
#include "levelmanager.h"
#include <stdexcept>

std::vector<Level*> LevelManager::levels;
std::unordered_map<std::string, std::unordered_map<std::string, Level*>> LevelManager::levelLookup;
std::unordered_map<std::string, std::vector<Level*>> LevelManager::levelList;
std::unordered_map<std::string, std::vector<std::string>> LevelManager::levelNameList;

void LevelManager::initialize() {
	levelLookup.insert({ "vanilla", std::unordered_map<std::string, Level*>() });
	levelLookup.insert({ "dev", std::unordered_map<std::string, Level*>() });
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
	levelNameList["vanilla"].push_back(l->getName());
	levelList["vanilla"].push_back(l);
	levelLookup["vanilla"].insert({ l->getName(), l });
}

Level* LevelManager::getLevelByName(std::string name) {
	return levelLookup["vanilla"][name];
}

std::string LevelManager::getLevelName(int index) {
	return levelNameList["vanilla"][index];
}

int LevelManager::getNumLevelTypes() {
	return levelNameList["vanilla"].size();
}

void LevelManager::addSpecialLevelToHashmap(std::string type, Level* l) {
	if (levelLookup.find(type) == levelLookup.end()) {
		levelLookup.insert({ type, std::unordered_map<std::string, Level*>() });
	}

	levelNameList[type].push_back(l->getName());
	levelList[type].push_back(l);
	levelLookup[type].insert({ l->getName(), l });
}

Level* LevelManager::getSpecialLevelByName(std::string type, std::string name) {
	if (levelLookup.find(type) == levelLookup.end()) {
		throw std::domain_error("level type \"" + type + "\" unknown!");
	}
	return levelLookup[type][name];
}

std::string LevelManager::getSpecialLevelName(std::string type, int index) {
	if (levelLookup.find(type) == levelLookup.end()) {
		throw std::domain_error("level type \"" + type + "\" unknown!");
	}
	return levelNameList[type][index];
}

int LevelManager::getNumSpecialLevelTypes(std::string type) {
	if (levelLookup.find(type) == levelLookup.end()) {
		throw std::domain_error("level type \"" + type + "\" unknown!");
	}
	return levelNameList[type].size();
}
