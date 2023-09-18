#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "level.h"
#include "custom-level-interpreter.h"

typedef Level* (*LevelFunction)(void);
typedef LevelEffect* (*LevelEffectFunction)(const GenericFactoryConstructionData&);

class LevelManager {
	friend class ResetThings;

private:
	static std::vector<Level*> levels; //active levels
	static void clearLevels(); //for ResetThings

	static std::unordered_map<std::string, std::unordered_map<std::string, LevelFunction>> levelLookup;
	static std::unordered_map<std::string, std::unordered_map<std::string, LevelEffectFunction>> levelEffectLookup;
	static std::unordered_map<std::string, std::vector<std::string>> levelNameList;
	static std::unordered_map<std::string, std::vector<std::string>> levelEffectNameList;

	static std::vector<std::string> protectedTypes; //types not allowed to be used (by custom levels)
	static std::unordered_map<std::string, std::unordered_map<std::string, CustomLevel*>> customLevelLookup;
	static std::unordered_map<std::string, std::vector<std::string>> customLevelNameList;

public:
	static void initialize();
	static Level* getLevel(unsigned int index); //why would this be needed
	static LevelEffect* getLevelEffect(unsigned int level_index, unsigned int index); //why would this be needed, like, ever?
	static void pushLevel(std::string type, std::string name);
	static unsigned int getNumLevels() { return levels.size(); }
	//static unsigned int getLevel_numEffects(unsigned int index) { return levels[index]->effects.size(); }
	//static void deleteLevel(unsigned int index);

	static void addLevelFactory(LevelFunction); //gets the types from the level
	static LevelFunction getLevelFactory(std::string type, std::string name); //TODO: remove?
	static std::string getLevelName(std::string type, unsigned int index);
	static unsigned int getNumLevelTypes(std::string type);
	static std::string levelWeightedSelect(std::string type); //returns name of level

	static void addLevelEffectFactory(LevelEffectFunction); //gets the types from the level effect
	static LevelEffectFunction getLevelEffectFactory(std::string type, std::string name);
	static std::string getLevelEffectName(std::string type, unsigned int index);
	static unsigned int getNumLevelEffectTypes(std::string type);

	static std::string checkCustomLevelTypesAgainstProtectedTypes(const std::vector<std::string>&) noexcept; //returns first bad type ("" if all good)
	static void addCustomLevel(std::string name, const std::vector<std::string>& types, CustomLevel* l);

private:
	LevelManager() = delete;
	LevelManager(const LevelManager&) = delete;
};
