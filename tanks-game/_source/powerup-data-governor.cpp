#include "powerup-data-governor.h"

#include <stdexcept>
#include <algorithm> //std::find

std::unordered_map<std::string, std::unordered_map<std::string, PowerFunction>> PowerupDataGovernor::powerLookup;
std::unordered_map<std::string, std::vector<std::string>> PowerupDataGovernor::powerNameList;

std::vector<std::string> PowerupDataGovernor::protectedTypes = { "null", "vanilla", "vanilla-extra", "random-vanilla", "old", "random-old", "supermix-vanilla", "ultimate-vanilla", "dev", "random-dev" };
std::unordered_map<std::string, std::unordered_map<std::string, CustomPower*>> PowerupDataGovernor::customPowerLookup;
std::unordered_map<std::string, std::vector<std::string>> PowerupDataGovernor::customPowerNameList;

void PowerupDataGovernor::initialize() {
	powerLookup.insert({ "vanilla", std::unordered_map<std::string, PowerFunction>() });
	powerLookup.insert({ "vanilla-extra", std::unordered_map<std::string, PowerFunction>() }); //shotgun, mines, tracking, fire?, barrier?
	powerLookup.insert({ "random-vanilla", std::unordered_map<std::string, PowerFunction>() });
	powerLookup.insert({ "old", std::unordered_map<std::string, PowerFunction>() }); //powers from JS (they may also be in vanilla)
	powerLookup.insert({ "random-old", std::unordered_map<std::string, PowerFunction>() });
	powerLookup.insert({ "supermix", std::unordered_map<std::string, PowerFunction>() }); //these powerups are for godmode
	powerLookup.insert({ "supermix-vanilla", std::unordered_map<std::string, PowerFunction>() }); //(godmode actually uses this one)
	powerLookup.insert({ "ultimate", std::unordered_map<std::string, PowerFunction>() }); //ultimate powerups (say, at the center of a level) (wallhack and godmode)
	powerLookup.insert({ "ultimate-vanilla", std::unordered_map<std::string, PowerFunction>() });
	powerLookup.insert({ "random", std::unordered_map<std::string, PowerFunction>() }); //general random
	powerLookup.insert({ "dev", std::unordered_map<std::string, PowerFunction>() });
	powerLookup.insert({ "random-dev", std::unordered_map<std::string, PowerFunction>() });
}

std::string PowerupDataGovernor::checkCustomPowerTypesAgainstProtectedTypes(const std::vector<std::string>& types) noexcept {
	for (int i = 0; i < protectedTypes.size(); i++) {
		if (std::find(types.begin(), types.end(), protectedTypes[i]) != types.end()) {
			return protectedTypes[i];
		}
	}
	return "";
}

void PowerupDataGovernor::addCustomPower(std::string name, const std::vector<std::string>& types, CustomPower* p) {
	if (std::find(types.begin(), types.end(), "null") != types.end()) { [[unlikely]]
		throw std::runtime_error("power " + name + " includes \"null\" type, which is not allowed");
	}
	for (int i = 0; i < types.size(); i++) {
		customPowerLookup[types[i]].insert({ p->getName(), p });
		customPowerNameList[types[i]].push_back(name);
	}
}

bool PowerupDataGovernor::doesPowerExist(std::string type, std::string name) noexcept {
	if (powerLookup.find(type) == powerLookup.end()) {
		if (customPowerLookup.find(type) != customPowerLookup.end()) {
			if (customPowerLookup[type].find(name) != customPowerLookup[type].end()) {
				return true;
			}
			return false;
		}
		return false;
	}
	if (powerLookup[type].find(name) != powerLookup[type].end()) {
		return true;
	}
	return false;
}


void PowerupDataGovernor::addPowerFactory(PowerFunction factory) {
	Power* p = factory();
	std::vector<std::string> types = p->getPowerTypes();
	if (std::find(types.begin(), types.end(), "null") != types.end()) {
		std::string name = p->getName();
		delete p;
		throw std::runtime_error("power " + name + " includes \"null\" type, which is not allowed");
	}
	for (int i = 0; i < types.size(); i++) {
		powerLookup[types[i]].insert({ p->getName(), factory });
		powerNameList[types[i]].push_back(p->getName());
	}
	delete p;
}

Power* PowerupDataGovernor::getPower(std::string type, std::string name) {
	//TODO: do doesPowerExist()?
	if (powerLookup.find(type) == powerLookup.end()) {
		//custom powers
		if (customPowerLookup.find(type) != customPowerLookup.end()) {
			if (customPowerLookup[type].find(name) != customPowerLookup[type].end()) {
				return customPowerLookup[type][name]->factory();
			}
			throw std::runtime_error("power name \"" + name + "\" (with type \"" + type + "\") unknown!");
		}
		throw std::runtime_error("power type \"" + type + "\" unknown!");
	} else {
		//regular powers
		if (powerLookup[type].find(name) == powerLookup[type].end()) {
			throw std::runtime_error("power name \"" + name + "\" (with type \"" + type + "\") unknown!");
		}
		return powerLookup[type][name]();
	}
}

std::string PowerupDataGovernor::getPowerName(std::string type, unsigned int index) {
	if (powerLookup.find(type) == powerLookup.end()) {
		if (customPowerLookup.find(type) != customPowerLookup.end()) {
			if (index >= customPowerNameList[type].size()) {
				throw std::range_error("power index " + std::to_string(index) + " is too large!");
			}
			return customPowerNameList[type][index];
		}
		throw std::runtime_error("power type \"" + type + "\" unknown!");
	}
	if (index >= powerNameList[type].size()) {
		throw std::range_error("power index " + std::to_string(index) + " is too large!");
	}
	return powerNameList[type][index];
}

unsigned int PowerupDataGovernor::getNumPowerTypes(std::string type) {
	if (powerLookup.find(type) == powerLookup.end()) {
		if (customPowerLookup.find(type) != customPowerLookup.end()) {
			return customPowerNameList[type].size();
		}
		throw std::runtime_error("power type \"" + type + "\" unknown!");
	}
	return powerNameList[type].size();
}
