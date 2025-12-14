#pragma once
#include <vector>
#include "level.h"

class LevelManager {
	friend class ResetThings;

private:
	static std::vector<Level*> levels; //active levels
	static void clearLevels(); //for ResetThings
	//sequel note: don't use a list of levels, that's stupid; there should only be one level in play; levels should be able to choose their size
	//a list was used because in JS, you could just keep pushing levels, since they were basically a collection of "push walls here" commands
	//also I'm not a fan of hard coding "only one level in play", but in this case it *really* makes sense
	//future note: Stage Morph?

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
	static int customLevelWeightedSelect(const std::vector<std::pair<std::pair<std::string, std::string>, float>>& customLevelList); //requires at least one level to have a weight >0

	//[[nodiscard]] static Level* makeLevel(std::string type, std::string name); //no point
	[[nodiscard]] static LevelEffect* makeLevelEffect(std::string type, std::string name, const GenericFactoryConstructionData& data);

private:
	LevelManager() = delete;
	LevelManager(const LevelManager&) = delete;
};
