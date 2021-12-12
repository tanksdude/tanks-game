#include "tankmanager.h"

std::vector<Tank*> TankManager::tanks;

void TankManager::initialize() {
	return;
}

Tank* TankManager::getTank(int index) {
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
void TankManager::deleteTank(int index) {
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
