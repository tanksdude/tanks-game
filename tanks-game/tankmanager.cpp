#pragma once
#include "tankmanager.h"

std::vector<Tank*> TankManager::tanks;
void TankManager::initialize() {
	return;
}

Tank* const TankManager::getTank(int index) {
	return tanks[index];
}

void TankManager::pushTank(Tank* t) {
	tanks.push_back(t);
}

/*
void TankManager::deleteTank(int index) {
	delete tanks[index];
	tanks.erase(tanks.begin() + index);
}
*/