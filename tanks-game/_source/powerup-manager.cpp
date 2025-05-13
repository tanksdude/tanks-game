#include "powerup-manager.h"

#include "powerup-data-governor.h"
//#include "game-manager.h"

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
	//GameManager::pushObject(p);
}
//static void pushPowerup(double x, double y, std::string type, std::string name); //TODO: these should probably be added
//static void pushPowerup(double x, double y, std::string type, const std::string* names, int num);
//static void pushPowerup(double x, double y, const std::string* types, const std::string* names, int num);

void PowerupManager::deletePowerup(unsigned int index) {
	//PowerSquare* p = powerups[index];
	//GameManager::deleteObjectByID(p->getGameID());
	delete powerups[index];
	powerups.erase(powerups.begin() + index);
}

void PowerupManager::deletePowerupByID(Game_ID gameID) {
	//GameManager::deleteObjectByID(gameID);
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
