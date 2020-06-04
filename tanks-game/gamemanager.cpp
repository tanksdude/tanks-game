#include "gamemanager.h"

long GameManager::nextID = -1;
void GameManager::initialize() {
	//nothing
}

long GameManager::getNextID() {
	nextID++;
	return nextID;
}
