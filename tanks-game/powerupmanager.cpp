#include "powerupmanager.h"
#include <stdexcept>

std::vector<PowerSquare*> PowerupManager::powerups; //active powersquares
std::unordered_map<std::string, std::unordered_map<std::string, PowerFunction>> PowerupManager::powerLookup;
std::unordered_map<std::string, std::vector<PowerFunction>> PowerupManager::powerList;
std::unordered_map<std::string, std::vector<std::string>> PowerupManager::powerNameList;

void PowerupManager::initialize() {
	powerLookup.insert({ "vanilla", std::unordered_map<std::string, PowerFunction>() });
	powerLookup.insert({ "vanilla-extra", std::unordered_map<std::string, PowerFunction>() }); //shotgun, mines, tracking, fire?, life?
	powerLookup.insert({ "random-vanilla", std::unordered_map<std::string, PowerFunction>() });
	powerLookup.insert({ "random", std::unordered_map<std::string, PowerFunction>() }); //general random (requires the power to manually insert itself here)
	powerLookup.insert({ "old", std::unordered_map<std::string, PowerFunction>() }); //probably just includes the versions of the powers from JS
	powerLookup.insert({ "random-old", std::unordered_map<std::string, PowerFunction>() });
	powerLookup.insert({ "dev", std::unordered_map<std::string, PowerFunction>() });
	powerLookup.insert({ "random-dev", std::unordered_map<std::string, PowerFunction>() }); //would this be used?
}

PowerSquare* PowerupManager::getPowerup(int index) {
	return powerups[index];
}

void PowerupManager::pushPowerup(PowerSquare* p) {
	powerups.push_back(p);
}

void PowerupManager::deletePowerup(int index) {
	delete powerups[index];
	powerups.erase(powerups.begin() + index);
}

void PowerupManager::clearPowerups() {
	for (int i = 0; i < powerups.size(); i++) {
		delete powerups[i];
	}
	powerups.clear();
}


void PowerupManager::addPowerFactory(PowerFunction factory) {
	Power* p = factory();
	std::vector<std::string> types = p->getPowerTypes();
	for (int i = 0; i < types.size(); i++) {
		/*
		//is this necessary?
		if (powerLookup.find(types[i]) == powerLookup.end()) {
			powerLookup.insert({ types[i], std::unordered_map<std::string, PowerFunction>() });
		}
		*/
		powerList[types[i]].push_back(factory);
		powerLookup[types[i]].insert({ p->getName(), factory });
		powerNameList[types[i]].push_back(p->getName());
	}
	delete p;
}

PowerFunction PowerupManager::getPowerFactory(std::string type, std::string name) {
	if (powerLookup.find(type) == powerLookup.end()) {
		throw std::domain_error("power type \"" + type + "\" unknown!");
	}
	return powerLookup[type][name];
}

std::string PowerupManager::getPowerName(std::string type, int index) {
	if (powerLookup.find(type) == powerLookup.end()) {
		throw std::domain_error("power type \"" + type + "\" unknown!");
	}
	return powerNameList[type][index];
}

int PowerupManager::getNumPowerTypes(std::string type) {
	if (powerLookup.find(type) == powerLookup.end()) {
		throw std::domain_error("power type \"" + type + "\" unknown!");
	}
	return powerNameList[type].size();
}
