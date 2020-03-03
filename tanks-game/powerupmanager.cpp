#pragma once
#include "powerupmanager.h"

std::vector<PowerSquare*> PowerupManager::powerups; //active powersquares
std::unordered_map<std::string, PowerFunction> PowerupManager::powerLookup;
std::vector<PowerFunction> PowerupManager::powerList;
std::vector<std::string> PowerupManager::powerNameList;

void PowerupManager::initialize() {
	return;
}

PowerSquare* const PowerupManager::getPowerup(int index) {
	return powerups[index];
}

void PowerupManager::pushPowerup(PowerSquare* b) {
	powerups.push_back(b);
}

void PowerupManager::deletePowerup(int index) {
	delete powerups[index];
	powerups.erase(powerups.begin() + index);
}


void PowerupManager::addPowerFactory(PowerFunction factory) {
	powerList.push_back(factory);
	Power* p = factory();
	powerLookup.insert({ p->getName(), factory });
	powerNameList.push_back(p->getName());
	delete p;
}

PowerFunction PowerupManager::getPowerFactory(std::string name) {
	return powerLookup[name];
}

std::string PowerupManager::getPowerName(int index) {
	return powerNameList[index];
}

int PowerupManager::getNumPowerTypes() {
	return powerNameList.size();
}