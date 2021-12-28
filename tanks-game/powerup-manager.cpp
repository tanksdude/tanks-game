#include "powerup-manager.h"
#include <stdexcept>

std::vector<PowerSquare*> PowerupManager::powerups; //active powersquares

std::unordered_map<std::string, std::unordered_map<std::string, PowerFunction>> PowerupManager::powerLookup;
std::unordered_map<std::string, std::vector<std::string>> PowerupManager::powerNameList;

void PowerupManager::initialize() {
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
	powerLookup.insert({ "random-dev", std::unordered_map<std::string, PowerFunction>() }); //would this be used?
}

PowerSquare* PowerupManager::getPowerup(int index) {
	return powerups[index];
}

PowerSquare* PowerupManager::getPowerupByID(Game_ID gameID) {
	for (int i = 0; i < powerups.size(); i++) {
		if (powerups[i]->getGameID() == gameID) {
			return powerups[i];
		}
	}
	return nullptr;
}

void PowerupManager::pushPowerup(PowerSquare* p) {
	powerups.push_back(p);
}

void PowerupManager::deletePowerup(int index) {
	delete powerups[index];
	powerups.erase(powerups.begin() + index);
}

void PowerupManager::deletePowerupByID(Game_ID gameID) {
	for (int i = 0; i < powerups.size(); i++) {
		if (powerups[i]->getGameID() == gameID) {
			deletePowerup(i);
			break;
		}
	}
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

std::vector<Rect*> PowerupManager::getPowerSquareCollisionList() {
	return std::vector<Rect*>(powerups.begin(), powerups.end());
}
