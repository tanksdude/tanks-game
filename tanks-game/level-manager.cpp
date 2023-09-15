#include "level-manager.h"
#include <stdexcept>
#include "custom-level-interpreter.h"
#include "mylib.h" //weightedSelect

std::vector<Level*> LevelManager::levels;

std::unordered_map<std::string, std::unordered_map<std::string, LevelFunction>> LevelManager::levelLookup;
std::unordered_map<std::string, std::unordered_map<std::string, LevelEffectFunction>> LevelManager::levelEffectLookup;
std::unordered_map<std::string, std::vector<std::string>> LevelManager::levelNameList;
std::unordered_map<std::string, std::vector<std::string>> LevelManager::levelEffectNameList;

std::unordered_map<std::string, std::vector<std::string>> LevelManager::customLevelNameList;
std::vector<std::string> LevelManager::protectedTypes = { "null", "vanilla", "vanilla-extra", "random-vanilla", "old", "random-old", "dev", "random-dev" };

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

std::string LevelManager::checkCustomLevelTypesAgainstProtectedTypes(const std::vector<std::string>& types) noexcept {
	for (int i = 0; i < protectedTypes.size(); i++) {
		if (std::find(types.begin(), types.end(), protectedTypes[i]) != types.end()) {
			return protectedTypes[i];
		}
	}
	return "";
}

void LevelManager::addCustomLevel(std::string name, const std::vector<std::string>& types) {
	for (int i = 0; i < types.size(); i++) {
		if (types[i] == "null") {
			throw std::runtime_error("level " + name + " includes \"null\" type, which is not allowed");
		} else { [[likely]]
			customLevelNameList[types[i]].push_back(name);
		}
	}
}

Level* LevelManager::getLevel(int index) {
	return levels[index];
}

LevelEffect* LevelManager::getLevelEffect(int level_index, int index) {
	return levels[level_index]->effects[index];
}

void LevelManager::pushLevel(std::string type, std::string name) {
	if (levelLookup.find(type) == levelLookup.end()) {
		//custom levels
		levels.push_back(CustomLevelInterpreter::factory(type, name));
	} else {
		//regular levels
		levels.push_back(LevelManager::getLevelFactory(type, name)());
	}
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
		if (types[i] == "null") {
			std::string name = l->getName();
			delete l;
			throw std::runtime_error("level " + name + " includes \"null\" type, which is not allowed");
		} else { [[likely]]
			levelLookup[types[i]].insert({ l->getName(), factory });
			levelNameList[types[i]].push_back(l->getName());
		}
	}
	delete l;
}

LevelFunction LevelManager::getLevelFactory(std::string type, std::string name) {
	if (levelLookup.find(type) == levelLookup.end()) {
		throw std::runtime_error("level type \"" + type + "\" unknown!");
	}
	if (levelLookup[type].find(name) == levelLookup[type].end()) {
		throw std::runtime_error("level name \"" + name + "\" (with type \"" + type + "\") unknown!");
	}
	return levelLookup[type][name];
}

std::string LevelManager::getLevelName(std::string type, unsigned int index) {
	if (levelLookup.find(type) == levelLookup.end()) {
		if (CustomLevelInterpreter::customLevelLookup.find(type) != CustomLevelInterpreter::customLevelLookup.end()) {
			if (index >= customLevelNameList[type].size()) {
				throw std::range_error("level index " + std::to_string(index) + " is too large!");
			}
			return customLevelNameList[type][index];
		}
		throw std::runtime_error("level type \"" + type + "\" unknown!");
	}
	if (index >= levelNameList[type].size()) {
		throw std::range_error("level index " + std::to_string(index) + " is too large!");
	}
	return levelNameList[type][index];
}

unsigned int LevelManager::getNumLevelTypes(std::string type) {
	if (levelLookup.find(type) == levelLookup.end()) {
		if (CustomLevelInterpreter::customLevelLookup.find(type) != CustomLevelInterpreter::customLevelLookup.end()) {
			return customLevelNameList[type].size();
		}
		throw std::runtime_error("level type \"" + type + "\" unknown!");
	}
	return levelNameList[type].size();
}

std::string LevelManager::levelWeightedSelect(std::string levelPlaylist) {
	if (CustomLevelInterpreter::customLevelLookup.find(levelPlaylist) == CustomLevelInterpreter::customLevelLookup.end()) {
		std::vector<float> levelWeights;
		levelWeights.reserve(LevelManager::getNumLevelTypes(levelPlaylist));
		for (unsigned int i = 0; i < LevelManager::getNumLevelTypes(levelPlaylist); i++) {
			std::string n = LevelManager::getLevelName(levelPlaylist, i);
			Level* l = LevelManager::getLevelFactory(levelPlaylist, n)();
			levelWeights.push_back(l->getWeights()[levelPlaylist]);
			delete l;
		}
		int levelIndex = weightedSelect<float>(levelWeights.data(), levelWeights.size());
		return LevelManager::getLevelName(levelPlaylist, levelIndex);
	} else {
		std::vector<float> levelWeights;
		levelWeights.reserve(LevelManager::getNumLevelTypes(levelPlaylist));
		for (unsigned int i = 0; i < LevelManager::getNumLevelTypes(levelPlaylist); i++) {
			std::string n = LevelManager::getLevelName(levelPlaylist, i);
			Level* l = CustomLevelInterpreter::customLevelLookup[levelPlaylist][n];
			levelWeights.push_back(l->getWeights()[levelPlaylist]);
			//delete l;
		}
		int levelIndex = weightedSelect<float>(levelWeights.data(), levelWeights.size());
		return LevelManager::getLevelName(levelPlaylist, levelIndex);
	}
}


void LevelManager::addLevelEffectFactory(LevelEffectFunction factory) {
	GenericFactoryConstructionData constructionData;
	LevelEffect* le = factory(constructionData);
	std::vector<std::string> types = le->getLevelEffectTypes();
	for (int i = 0; i < types.size(); i++) {
		if (types[i] == "null") {
			std::string name = le->getName();
			delete le;
			throw std::runtime_error("level effect " + name + " includes \"null\" type, which is not allowed");
		} else { [[likely]]
			levelEffectLookup[types[i]].insert({ le->getName(), factory });
			levelEffectNameList[types[i]].push_back(le->getName());
		}
	}
	delete le;
}

LevelEffectFunction LevelManager::getLevelEffectFactory(std::string type, std::string name) {
	if (levelEffectLookup.find(type) == levelEffectLookup.end()) {
		throw std::runtime_error("level effect type \"" + type + "\" unknown!");
	}
	if (levelEffectLookup[type].find(name) == levelEffectLookup[type].end()) {
		throw std::runtime_error("level effect name \"" + name + "\" (with type \"" + type + "\") unknown!");
	}
	return levelEffectLookup[type][name];
}

std::string LevelManager::getLevelEffectName(std::string type, unsigned int index) {
	if (levelEffectLookup.find(type) == levelEffectLookup.end()) {
		throw std::runtime_error("level effect type \"" + type + "\" unknown!");
	}
	if (index >= levelEffectNameList[type].size()) {
		throw std::range_error("level effect index " + std::to_string(index) + " is too large!");
	}
	return levelEffectNameList[type][index];
}

unsigned int LevelManager::getNumLevelEffectTypes(std::string type) {
	if (levelEffectLookup.find(type) == levelEffectLookup.end()) {
		throw std::runtime_error("level effect type \"" + type + "\" unknown!");
	}
	return levelEffectNameList[type].size();
}
