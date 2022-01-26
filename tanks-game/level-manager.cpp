#include "level-manager.h"
#include <stdexcept>

std::vector<Level*> LevelManager::levels;

std::unordered_map<std::string, std::unordered_map<std::string, LevelFunction>> LevelManager::levelLookup;
std::unordered_map<std::string, std::unordered_map<std::string, LevelEffectFunction>> LevelManager::levelEffectLookup;
std::unordered_map<std::string, std::vector<std::string>> LevelManager::levelNameList;
std::unordered_map<std::string, std::vector<std::string>> LevelManager::levelEffectNameList;

void LevelManager::initialize() {
	levelLookup.insert({ "vanilla", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "vanilla-extra", std::unordered_map<std::string, LevelFunction>() }); //... what does this include?
	levelLookup.insert({ "random-vanilla", std::unordered_map<std::string, LevelFunction>() }); //can include vanilla-extra but probably won't
	levelLookup.insert({ "old", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "random-old", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "random", std::unordered_map<std::string, LevelFunction>() }); //general random
	levelLookup.insert({ "dev", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "random-dev", std::unordered_map<std::string, LevelFunction>() }); //would this be used?

	levelEffectLookup.insert({ "vanilla", std::unordered_map<std::string, LevelEffectFunction>() });
	levelEffectLookup.insert({ "vanilla-extra", std::unordered_map<std::string, LevelEffectFunction>() }); //most would probably fall here...
	levelEffectLookup.insert({ "random-vanilla", std::unordered_map<std::string, LevelEffectFunction>() });
	levelEffectLookup.insert({ "old", std::unordered_map<std::string, LevelEffectFunction>() });
	levelEffectLookup.insert({ "random-old", std::unordered_map<std::string, LevelEffectFunction>() });
	levelEffectLookup.insert({ "random", std::unordered_map<std::string, LevelEffectFunction>() }); //this could be a little terrifying
	levelEffectLookup.insert({ "dev", std::unordered_map<std::string, LevelEffectFunction>() });
	levelEffectLookup.insert({ "random-dev", std::unordered_map<std::string, LevelEffectFunction>() }); //would this be used?
}

Level* LevelManager::getLevel(int index) {
	return levels[index];
}

LevelEffect* LevelManager::getLevelEffect(int level_index, int index) {
	return levels[level_index]->effects[index];
}

void LevelManager::pushLevel(std::string type, std::string name) {
	levels.push_back(LevelManager::getLevelFactory(type, name)());
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
		levelLookup[types[i]].insert({ l->getName(), factory });
		levelNameList[types[i]].push_back(l->getName());
	}
	delete l;
}

LevelFunction LevelManager::getLevelFactory(std::string type, std::string name) {
	if (levelLookup.find(type) == levelLookup.end()) {
		throw std::domain_error("level type \"" + type + "\" unknown!");
	}
	return levelLookup[type][name];
}

std::string LevelManager::getLevelName(std::string type, int index) {
	if (levelLookup.find(type) == levelLookup.end()) {
		throw std::domain_error("level type \"" + type + "\" unknown!");
	}
	return levelNameList[type][index];
}

int LevelManager::getNumLevelTypes(std::string type) {
	if (levelLookup.find(type) == levelLookup.end()) {
		throw std::domain_error("level type \"" + type + "\" unknown!");
	}
	return levelNameList[type].size();
}


void LevelManager::addLevelEffectFactory(LevelEffectFunction factory) {
	GenericFactoryConstructionData constructionData;
	LevelEffect* le = factory(constructionData);
	std::vector<std::string> types = le->getLevelEffectTypes();
	for (int i = 0; i < types.size(); i++) {
		/*
		//is this necessary?
		if (levelEffectLookup.find(types[i]) == levelEffectLookup.end()) {
			levelEffectLookup.insert({ types[i], std::unordered_map<std::string, LevelEffectFunction>() });
		}
		*/
		levelEffectLookup[types[i]].insert({ le->getName(), factory });
		levelEffectNameList[types[i]].push_back(le->getName());
	}
	delete le;
}

LevelEffectFunction LevelManager::getLevelEffectFactory(std::string type, std::string name) {
	if (levelEffectLookup.find(type) == levelEffectLookup.end()) {
		throw std::domain_error("level effect type \"" + type + "\" unknown!");
	}
	return levelEffectLookup[type][name];
}

std::string LevelManager::getLevelEffectName(std::string type, int index) {
	if (levelEffectLookup.find(type) == levelEffectLookup.end()) {
		throw std::domain_error("level effect type \"" + type + "\" unknown!");
	}
	return levelEffectNameList[type][index];
}

int LevelManager::getNumLevelEffectTypes(std::string type) {
	if (levelEffectLookup.find(type) == levelEffectLookup.end()) {
		throw std::domain_error("level effect type \"" + type + "\" unknown!");
	}
	return levelEffectNameList[type].size();
}
