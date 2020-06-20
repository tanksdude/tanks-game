#include "gamemanager.h"

long GameManager::nextID = -1;
double GameManager::tickCount = 0;

void GameManager::initialize() {
	//nothing
}

void GameManager::tick() {
	tickCount++;
	//this is probably all it's ever going to do
}

void GameManager::reset() {
	tickCount = 0;
}

long GameManager::getNextID() {
	nextID++;
	return nextID;
}

double GameManager::getTickCount() {
	return tickCount;
}
