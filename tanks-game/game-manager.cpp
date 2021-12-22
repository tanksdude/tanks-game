#include "game-manager.h"

Game_ID GameManager::nextID = -1;
double GameManager::tickCount = 0;

void GameManager::Initialize() {
	//nothing
}

void GameManager::Tick() {
	tickCount++;
	//this is probably all it's ever going to do
}

void GameManager::Reset() {
	tickCount = 0;
}

Game_ID GameManager::getNextID() {
	nextID++;
	return nextID;
}

double GameManager::getTickCount() {
	return tickCount;
}
