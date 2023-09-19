#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "power.h"

typedef Power* (*PowerFunction)(void);

class PowerupDataGovernor {
private:
	static std::unordered_map<std::string, std::unordered_map<std::string, PowerFunction>> powerLookup;
	static std::unordered_map<std::string, std::vector<std::string>> powerNameList;

	static std::vector<std::string> protectedTypes; //types not allowed to be used (by custom powers)

public:
	static void initialize();

	static void addPowerFactory(PowerFunction);
	static PowerFunction getPowerFactory(std::string type, std::string name);
	static std::string getPowerName(std::string type, unsigned int index);
	static unsigned int getNumPowerTypes(std::string type);

	static std::string checkCustomPowerTypesAgainstProtectedTypes(const std::vector<std::string>&) noexcept; //returns first bad type ("" if all good)
	//static void addCustomPower(std::string name, const std::vector<std::string>& types, CustomPower* p);

private:
	PowerupDataGovernor() = delete;
	PowerupDataGovernor(const PowerupDataGovernor&) = delete;
};
