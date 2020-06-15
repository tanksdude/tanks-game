#include "levelmanager.h"
#include <stdexcept>

std::vector<Level*> LevelManager::levels;
std::unordered_map<std::string, std::unordered_map<std::string, LevelFunction>> LevelManager::levelLookup;
std::unordered_map<std::string, std::vector<LevelFunction>> LevelManager::levelList;
std::unordered_map<std::string, std::vector<std::string>> LevelManager::levelNameList;

void LevelManager::initialize() {
	levelLookup.insert({ "vanilla", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "vanilla-extra", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "random", std::unordered_map<std::string, LevelFunction>() }); //all/general random
	levelLookup.insert({ "old", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "random-old", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "dev", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "random-dev", std::unordered_map<std::string, LevelFunction>() }); //would be used?
}

Level* LevelManager::getLevel(int index) {
	return levels[index];
}

LevelEffect* LevelManager::getLevelEffect(int level_index, int index) {
	return levels[level_index]->effects[index];
}

void LevelManager::pushLevel(std::string name) {
	levels.push_back(LevelManager::getLevelFactory(name)());
}

void LevelManager::pushSpecialLevel(std::string type, std::string name) {
	levels.push_back(LevelManager::getSpecialLevelFactory(type, name)());
}

/*
void LevelManager::deleteLevel(int index) {
	delete levels[index];
	levels.erase(levels.begin() + index);
}
*/

void LevelManager::clearLevels() {
	//levels are not singletons because they may have some variables that get reset, so they need to be deleted
	for (int i = 0; i < levels.size(); i++) {
		delete levels[i];
	}
	levels.clear();
}


void LevelManager::addLevelFactory(LevelFunction factory) {
	Level* l = factory();
	std::vector<std::string> types = l->getLevelTypes();
	for (int i = 0; i < types.size(); i++) {
		/*
		//is this necessary?
		if (levelLookup.find(types[i]) == levelLookup.end()) {
			levelLookup.insert({ types[i], std::unordered_map<std::string, LevelFunction>() });
		}
		*/
		levelList[types[i]].push_back(factory);
		levelLookup[types[i]].insert({ l->getName(), factory });
		levelNameList[types[i]].push_back(l->getName());
	}
	delete l;
}

LevelFunction LevelManager::getLevelFactory(std::string name) {
	return levelLookup["vanilla"][name];
}

std::string LevelManager::getLevelName(int index) {
	return levelNameList["vanilla"][index];
}

int LevelManager::getNumLevelTypes() {
	return levelNameList["vanilla"].size();
}

LevelFunction LevelManager::getSpecialLevelFactory(std::string type, std::string name) {
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
