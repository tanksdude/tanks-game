#pragma once
#include "powerupmanager.h"

std::vector<PowerSquare*> PowerupManager::powerups;
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