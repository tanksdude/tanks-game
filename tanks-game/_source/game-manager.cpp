#include "game-manager.h"

#include "mylib.h"
#include <stdexcept>
#include <iostream>
//#include <execution>

std::vector<GameThing*> GameManager::objectsInGame;

void GameManager::initializeObjectList() {
	//nothing
}

GameThing* GameManager::getObject(unsigned int index) {
	return objectsInGame[index];
}

GameThing* GameManager::getObjectByID(Game_ID gameID) {
	for (int i = 0; i < objectsInGame.size(); i++) {
		if (objectsInGame[i]->getGameID() == gameID) {
			return objectsInGame[i];
		}
	}
	return nullptr;
}

void GameManager::pushObject(GameThing* g) {
	//TODO?
	objectsInGame.push_back(g);
}

void GameManager::deleteObject(unsigned int index) {
	//TODO?
	objectsInGame.erase(objectsInGame.begin() + index);
}

void GameManager::deleteObjectByID(Game_ID gameID) {
	for (int i = 0; i < objectsInGame.size(); i++) {
		if (objectsInGame[i]->getGameID() == gameID) {
			deleteObject(i);
			break;
		}
	}
}

void GameManager::clearObjects() {
	//TODO?
	objectsInGame.clear();
}

void GameManager::updateEveryAABB() {
	for (int i = 0; i < objectsInGame.size(); i++) {
		objectsInGame[i]->updateAABB();
	}
	InsertionSort(objectsInGame.begin(), objectsInGame.end(), [](const GameThing* lhs, const GameThing* rhs) { return lhs->get_xStart() < rhs->get_xStart(); });
	//std::sort(std::execution::par, objectsInGame.begin(), objectsInGame.end(), [](const GameThing* lhs, const GameThing* rhs) { return lhs->get_xStart() < rhs->get_xStart(); });
	//as expected, that ^^^ is faster for banana spikes, otherwise slightly slower; in the future, try insertion on old stuff, quicksort on new stuff, then merge together
}

Game_ID GameManager::nextID = -1;
double GameManager::tickCount = 0;
BasicINIParser::BasicINIData GameManager::INI_file;
GameSettings GameManager::settings_file;

void GameManager::Initialize() {
	//does not initialize the INI or the settings
}

void GameManager::initializeINI(std::string path) {
	try {
		INI_file = BasicINIParser::ReadFile(path);
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Unexpected error: " << e.what() << std::endl;
		return;
	}
}

void GameManager::initializeSettings() {
	settings_file.Initialize(INI_file);
}

const BasicINIParser::BasicINIData& GameManager::get_INI() {
	return INI_file;
}
const GameSettings& GameManager::get_settings() {
	return settings_file;
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
