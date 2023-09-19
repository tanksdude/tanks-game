#include "powerup-manager.h"

#include "powerup-data-governor.h"

std::vector<PowerSquare*> PowerupManager::powerups; //active powersquares

void PowerupManager::initialize() {
	return;
}

PowerSquare* PowerupManager::getPowerup(unsigned int index) {
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

void PowerupManager::deletePowerup(unsigned int index) {
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

std::vector<Rect*> PowerupManager::getPowerupCollisionList() {
	return std::vector<Rect*>(powerups.begin(), powerups.end());
}
