#include "powerupmanager.h"
#include <stdexcept>

std::vector<PowerSquare*> PowerupManager::powerups; //active powersquares
std::unordered_map<std::string, std::unordered_map<std::string, PowerFunction>> PowerupManager::powerLookup;
std::unordered_map<std::string, std::vector<PowerFunction>> PowerupManager::powerList;
std::unordered_map<std::string, std::vector<std::string>> PowerupManager::powerNameList;

void PowerupManager::initialize() {
	powerLookup.insert({ "vanilla", std::unordered_map<std::string, PowerFunction>() });
	powerLookup.insert({ "dev", std::unordered_map<std::string, PowerFunction>() });
}

PowerSquare* const PowerupManager::getPowerup(int index) {
	return powerups[index];
}

void PowerupManager::pushPowerup(PowerSquare* p) {
	powerups.push_back(p);
}

void PowerupManager::deletePowerup(int index) {
	delete powerups[index];
	powerups.erase(powerups.begin() + index);
}


void PowerupManager::addPowerFactory(PowerFunction factory) {
	powerList["vanilla"].push_back(factory);
	Power* p = factory();
	powerLookup["vanilla"].insert({ p->getName(), factory });
	powerNameList["vanilla"].push_back(p->getName());
	delete p;
}

PowerFunction PowerupManager::getPowerFactory(std::string name) {
	return powerLookup["vanilla"][name];
}

std::string PowerupManager::getPowerName(int index) {
	return powerNameList["vanilla"][index];
}

int PowerupManager::getNumPowerTypes() {
	return powerNameList["vanilla"].size();
}

void PowerupManager::addSpecialPowerFactory(std::string type, PowerFunction factory) {
	if (powerLookup.find(type) == powerLookup.end()) {
		powerLookup.insert({ type, std::unordered_map<std::string, PowerFunction>() });
	}

	powerList[type].push_back(factory);
	Power* p = factory();
	powerLookup[type].insert({ p->getName(), factory });
	powerNameList[type].push_back(p->getName());
	delete p;
}

PowerFunction PowerupManager::getSpecialPowerFactory(std::string type, std::string name) {
	if (powerLookup.find(type) == powerLookup.end()) {
		throw std::domain_error("power type \"" + type + "\" unknown!");
	}
	return powerLookup[type][name];
}

std::string PowerupManager::getSpecialPowerName(std::string type, int index) {
	if (powerLookup.find(type) == powerLookup.end()) {
		throw std::domain_error("power type \"" + type + "\" unknown!");
	}
	return powerNameList[type][index];
}

int PowerupManager::getNumSpecialPowerTypes(std::string type) {
	if (powerLookup.find(type) == powerLookup.end()) {
		throw std::domain_error("power type \"" + type + "\" unknown!");
	}
	return powerNameList[type].size();
}
