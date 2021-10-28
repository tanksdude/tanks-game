#pragma once
#include "constants.h"

class GameManager {
	friend class DeveloperManager;
	friend class ResetThings;

private:
	static Game_ID nextID;
	static double tickCount; //double in case physics happens at a variable rate
	static void Reset();

public:
	static void Tick();
	static double getTickCount();
	static Game_ID getNextID();
	static void Initialize();

private:
	GameManager() {}
	GameManager(const GameManager&) {}
};
