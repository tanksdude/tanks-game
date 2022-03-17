#include "game-manager.h"
#include <iostream>
#include <stdexcept>

Game_ID GameManager::nextID = -1;
double GameManager::tickCount = 0;
BasicINIParser::BasicINIData GameManager::INI_file;

void GameManager::Initialize() {
	//does not initialize the INI
}

void GameManager::initializeINI(std::string path) {
	try {
		INI_file = BasicINIParser::readFile(path);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}
}

const BasicINIParser::BasicINIData& GameManager::get_INI() {
	//return (const BasicINIParser::BasicINIData&)INI_file;
	return INI_file;
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
