#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory> //std::unique_ptr

#include "level.h"
#include "custom-level-interpreter.h" //CustomLevel

typedef Level* (*LevelFunction)(void);
typedef LevelEffect* (*LevelEffectFunction)(const GenericFactoryConstructionData&);

class LevelDataGovernor {
	friend class LevelManager;

private:
	static std::unordered_map<std::string, std::unordered_map<std::string, LevelFunction>> levelLookup;
	static std::unordered_map<std::string, std::unordered_map<std::string, LevelEffectFunction>> levelEffectLookup;
	static std::unordered_map<std::string, std::vector<std::string>> levelNameList;
	static std::unordered_map<std::string, std::vector<std::string>> levelEffectNameList;

	static std::vector<std::string> protectedTypes; //types not allowed to be used (by custom levels)
	static std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<CustomLevel>>> customLevelLookup;
	static std::unordered_map<std::string, std::vector<std::string>> customLevelNameList;

public:
	static void initialize();

	static void addLevelFactory(LevelFunction); //gets the types from the level
	static LevelFunction getLevelFactory(std::string type, std::string name); //TODO: remove?
	static std::string getLevelName(std::string type, unsigned int index);
	static unsigned int getNumLevelTypes(std::string type);

	static void addLevelEffectFactory(LevelEffectFunction); //gets the types from the level effect
	static LevelEffectFunction getLevelEffectFactory(std::string type, std::string name);
	static std::string getLevelEffectName(std::string type, unsigned int index);
	static unsigned int getNumLevelEffectTypes(std::string type);

	static std::string checkCustomLevelTypesAgainstProtectedTypes(const std::vector<std::string>&) noexcept; //returns first bad type ("" if all good)
	static void addCustomLevel(std::string name, const std::vector<std::string>& types, CustomLevel* l);

private:
	LevelDataGovernor() = delete;
	LevelDataGovernor(const LevelDataGovernor&) = delete;
};
