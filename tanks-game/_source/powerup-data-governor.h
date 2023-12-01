#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "power.h"
#include "custom-power-interpreter.h" //CustomPower

typedef Power* (*PowerFunction)(void);

class PowerupDataGovernor {
private:
	static std::unordered_map<std::string, std::unordered_map<std::string, PowerFunction>> powerLookup;
	static std::unordered_map<std::string, std::vector<std::string>> powerNameList;

	static std::vector<std::string> protectedTypes; //types not allowed to be used (by custom powers)
	static std::unordered_map<std::string, std::unordered_map<std::string, CustomPower*>> customPowerLookup;
	static std::unordered_map<std::string, std::vector<std::string>> customPowerNameList;

public:
	static void initialize();

	static void addPowerFactory(PowerFunction);
	static Power* getPower(std::string type, std::string name);
	static std::string getPowerName(std::string type, unsigned int index);
	static unsigned int getNumPowerTypes(std::string type);

	static bool doesPowerExist(std::string type, std::string name) noexcept; //only used by CustomLevelInterpreter; everything else assumes the power exists
	static std::string checkCustomPowerTypesAgainstProtectedTypes(const std::vector<std::string>&) noexcept; //returns first bad type ("" if all good)
	static void addCustomPower(std::string name, const std::vector<std::string>& types, CustomPower* p);

private:
	PowerupDataGovernor() = delete;
	PowerupDataGovernor(const PowerupDataGovernor&) = delete;
};
