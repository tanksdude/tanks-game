#include "level-manager.h"

#include "mylib.h" //weightedSelect
#include "level-data-governor.h"

std::vector<Level*> LevelManager::levels;

void LevelManager::initialize() {
	return;
}

Level* LevelManager::getLevel(unsigned int index) {
	return levels[index];
}

LevelEffect* LevelManager::getLevelEffect(unsigned int level_index, unsigned int index) {
	return levels[level_index]->effects[index];
}

void LevelManager::pushLevel(std::string type, std::string name) {
	if (LevelDataGovernor::levelLookup.find(type) == LevelDataGovernor::levelLookup.end()) {
		//custom levels
		levels.push_back(LevelDataGovernor::customLevelLookup[type][name]->factory());
	} else {
		//regular levels
		levels.push_back(LevelDataGovernor::getLevelFactory(type, name)());
	}
}

/*
void LevelManager::deleteLevel(int index) {
	delete levels[index];
	levels.erase(levels.begin() + index);
}

void LevelManager::deleteLevelEffect(unsigned int level_index, unsigned int index) {
	delete levels[level_index]->effects[index];
	levels[level_index]->effects.erase(levels[level_index]->effects.begin() + index);
}
*/

std::string LevelManager::levelWeightedSelect(std::string levelPlaylist) {
	if (LevelDataGovernor::customLevelLookup.find(levelPlaylist) == LevelDataGovernor::customLevelLookup.end()) {
		std::vector<float> levelWeights;
		levelWeights.reserve(LevelDataGovernor::getNumLevelTypes(levelPlaylist));
		for (unsigned int i = 0; i < LevelDataGovernor::getNumLevelTypes(levelPlaylist); i++) {
			std::string n = LevelDataGovernor::getLevelName(levelPlaylist, i);
			Level* l = LevelDataGovernor::getLevelFactory(levelPlaylist, n)();
			levelWeights.push_back(l->getWeights()[levelPlaylist]);
			delete l;
		}
		int levelIndex = weightedSelect<float>(levelWeights.data(), levelWeights.size());
		return LevelDataGovernor::getLevelName(levelPlaylist, levelIndex);
	} else {
		std::vector<float> levelWeights;
		levelWeights.reserve(LevelDataGovernor::getNumLevelTypes(levelPlaylist));
		for (unsigned int i = 0; i < LevelDataGovernor::getNumLevelTypes(levelPlaylist); i++) {
			std::string n = LevelDataGovernor::getLevelName(levelPlaylist, i);
			CustomLevel* l = LevelDataGovernor::customLevelLookup[levelPlaylist][n];
			levelWeights.push_back(l->getWeights()[levelPlaylist]);
			//delete l;
		}
		int levelIndex = weightedSelect<float>(levelWeights.data(), levelWeights.size());
		return LevelDataGovernor::getLevelName(levelPlaylist, levelIndex);
	}
}

LevelEffect* LevelManager::makeLevelEffect(std::string type, std::string name, const GenericFactoryConstructionData& data) {
	return LevelDataGovernor::getLevelEffectFactory(type, name)(data);
}

void LevelManager::clearLevels() {
	for (int i = 0; i < levels.size(); i++) {
		delete levels[i];
	}
	levels.clear();
}
