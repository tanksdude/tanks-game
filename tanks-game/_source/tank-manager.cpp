#include "tank-manager.h"

std::vector<Tank*> TankManager::tanks;

void TankManager::initialize() {
	return;
}

Tank* TankManager::getTank(unsigned int index) {
	return tanks[index];
}

Tank* TankManager::getTankByID(Game_ID gameID) {
	for (int i = 0; i < tanks.size(); i++) {
		if (tanks[i]->getGameID() == gameID) {
			return tanks[i];
		}
	}
	return nullptr;
}

void TankManager::pushTank(Tank* t) {
	tanks.push_back(t);
}

/*
void TankManager::deleteTank(unsigned int index) {
	delete tanks[index];
	tanks.erase(tanks.begin() + index);
}

void TankManager::deleteTankByID(Game_ID gameID) {
	for (int i = 0; i < tanks.size(); i++) {
		if (tanks[i]->getGameID() == gameID) {
			deleteTank(i);
		}
	}
}
*/

void TankManager::clearTanks() {
	for (int i = 0; i < tanks.size(); i++) {
		delete tanks[i];
	}
	tanks.clear();
}

std::vector<Circle*> TankManager::getTankCollisionList() {
	return std::vector<Circle*>(tanks.begin(), tanks.end());
}
