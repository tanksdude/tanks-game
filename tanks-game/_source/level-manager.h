#pragma once
#include <vector>
#include "level.h"

class LevelManager {
	friend class ResetThings;

private:
	static std::vector<Level*> levels; //active levels
	static void clearLevels(); //for ResetThings

public:
	static void initialize();
	static Level* getLevel(unsigned int index); //why would this be needed
	static LevelEffect* getLevelEffect(unsigned int level_index, unsigned int index); //why would this be needed, like, ever?
	static void pushLevel(std::string type, std::string name);
	static unsigned int getNumLevels() { return levels.size(); }
	//static unsigned int getLevel_numEffects(unsigned int index) { return levels[index]->effects.size(); }
	//static void deleteLevel(unsigned int index);
	//static void deleteLevelEffect(unsigned int level_index, unsigned int index);
	static std::string levelWeightedSelect(std::string type);

	//[[nodiscard]] static Level* makeLevel(std::string type, std::string name); //no point
	[[nodiscard]] static LevelEffect* makeLevelEffect(std::string type, std::string name, const GenericFactoryConstructionData& data);

private:
	LevelManager() = delete;
	LevelManager(const LevelManager&) = delete;
};
