#include "level-data-governor.h"

#include <stdexcept>
#include <algorithm> //std::find

std::unordered_map<std::string, std::unordered_map<std::string, LevelFunction>> LevelDataGovernor::levelLookup;
std::unordered_map<std::string, std::unordered_map<std::string, LevelEffectFunction>> LevelDataGovernor::levelEffectLookup;
std::unordered_map<std::string, std::vector<std::string>> LevelDataGovernor::levelNameList;
std::unordered_map<std::string, std::vector<std::string>> LevelDataGovernor::levelEffectNameList;

std::vector<std::string> LevelDataGovernor::protectedTypes = { "null", "vanilla", "vanilla-extra", "random-vanilla", "old", "random-old", "dev", "random-dev" };
std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<CustomLevel>>> LevelDataGovernor::customLevelLookup;
std::unordered_map<std::string, std::vector<std::string>> LevelDataGovernor::customLevelNameList;

void LevelDataGovernor::initialize() {
	levelLookup.insert({ "vanilla", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "vanilla-extra", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "random-vanilla", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "old", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "random-old", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "random", std::unordered_map<std::string, LevelFunction>() }); //general random
	levelLookup.insert({ "dev", std::unordered_map<std::string, LevelFunction>() });
	levelLookup.insert({ "random-dev", std::unordered_map<std::string, LevelFunction>() });

	levelEffectLookup.insert({ "vanilla", std::unordered_map<std::string, LevelEffectFunction>() });
	levelEffectLookup.insert({ "vanilla-extra", std::unordered_map<std::string, LevelEffectFunction>() });
	levelEffectLookup.insert({ "random-vanilla", std::unordered_map<std::string, LevelEffectFunction>() });
	levelEffectLookup.insert({ "old", std::unordered_map<std::string, LevelEffectFunction>() });
	levelEffectLookup.insert({ "random-old", std::unordered_map<std::string, LevelEffectFunction>() });
	levelEffectLookup.insert({ "random", std::unordered_map<std::string, LevelEffectFunction>() }); //this could be a little terrifying
	levelEffectLookup.insert({ "dev", std::unordered_map<std::string, LevelEffectFunction>() });
	levelEffectLookup.insert({ "random-dev", std::unordered_map<std::string, LevelEffectFunction>() });
}

std::string LevelDataGovernor::checkCustomLevelTypesAgainstProtectedTypes(const std::vector<std::string>& types) noexcept {
	for (int i = 0; i < protectedTypes.size(); i++) {
		if (std::find(types.begin(), types.end(), protectedTypes[i]) != types.end()) {
			return protectedTypes[i];
		}
	}
	return "";
}

void LevelDataGovernor::addCustomLevel(std::string name, const std::vector<std::string>& types, CustomLevel* l) {
	if (std::find(types.begin(), types.end(), "null") != types.end()) { [[unlikely]]
		throw std::runtime_error("level " + name + " includes \"null\" type, which is not allowed");
	}
	for (int i = 0; i < types.size(); i++) {
		customLevelLookup[types[i]].insert({ l->getName(), std::unique_ptr<CustomLevel>(l) });
		customLevelNameList[types[i]].push_back(name);
	}
}


void LevelDataGovernor::addLevelFactory(LevelFunction factory) {
	Level* l = factory();
	std::vector<std::string> types = l->getLevelTypes();
	if (std::find(types.begin(), types.end(), "null") != types.end()) {
		std::string name = l->getName();
		delete l;
		throw std::runtime_error("level " + name + " includes \"null\" type, which is not allowed");
	}
	for (int i = 0; i < types.size(); i++) {
		levelLookup[types[i]].insert({ l->getName(), factory });
		levelNameList[types[i]].push_back(l->getName());
	}
	delete l;
}

LevelFunction LevelDataGovernor::getLevelFactory(std::string type, std::string name) {
	if (levelLookup.find(type) == levelLookup.end()) {
		throw std::runtime_error("level type \"" + type + "\" unknown!");
	}
	if (levelLookup[type].find(name) == levelLookup[type].end()) {
		throw std::runtime_error("level name \"" + name + "\" (with type \"" + type + "\") unknown!");
	}
	return levelLookup[type][name];
}

std::string LevelDataGovernor::getLevelName(std::string type, unsigned int index) {
	if (levelLookup.find(type) == levelLookup.end()) {
		if (customLevelLookup.find(type) != customLevelLookup.end()) {
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

unsigned int LevelDataGovernor::getNumLevelTypes(std::string type) {
	if (levelLookup.find(type) == levelLookup.end()) {
		if (customLevelLookup.find(type) != customLevelLookup.end()) {
			return customLevelNameList[type].size();
		}
		throw std::runtime_error("level type \"" + type + "\" unknown!");
	}
	return levelNameList[type].size();
}


void LevelDataGovernor::addLevelEffectFactory(LevelEffectFunction factory) {
	GenericFactoryConstructionData constructionData;
	LevelEffect* le = factory(constructionData);
	std::vector<std::string> types = le->getLevelEffectTypes();
	if (std::find(types.begin(), types.end(), "null") != types.end()) {
		std::string name = le->getName();
		delete le;
		throw std::runtime_error("level effect " + name + " includes \"null\" type, which is not allowed");
	}
	for (int i = 0; i < types.size(); i++) {
		levelEffectLookup[types[i]].insert({ le->getName(), factory });
		levelEffectNameList[types[i]].push_back(le->getName());
	}
	delete le;
}

LevelEffectFunction LevelDataGovernor::getLevelEffectFactory(std::string type, std::string name) {
	if (levelEffectLookup.find(type) == levelEffectLookup.end()) {
		throw std::runtime_error("level effect type \"" + type + "\" unknown!");
	}
	if (levelEffectLookup[type].find(name) == levelEffectLookup[type].end()) {
		throw std::runtime_error("level effect name \"" + name + "\" (with type \"" + type + "\") unknown!");
	}
	return levelEffectLookup[type][name];
}

std::string LevelDataGovernor::getLevelEffectName(std::string type, unsigned int index) {
	if (levelEffectLookup.find(type) == levelEffectLookup.end()) {
		throw std::runtime_error("level effect type \"" + type + "\" unknown!");
	}
	if (index >= levelEffectNameList[type].size()) {
		throw std::range_error("level effect index " + std::to_string(index) + " is too large!");
	}
	return levelEffectNameList[type][index];
}

unsigned int LevelDataGovernor::getNumLevelEffectTypes(std::string type) {
	if (levelEffectLookup.find(type) == levelEffectLookup.end()) {
		throw std::runtime_error("level effect type \"" + type + "\" unknown!");
	}
	return levelEffectNameList[type].size();
}
