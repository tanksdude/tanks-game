#pragma once
#include "constants.h"
#include "basic-ini-parser.h"

class GameManager {
	friend class DeveloperManager;
	friend class ResetThings;

private:
	static Game_ID nextID;
	static double tickCount; //double in case physics happens at a variable rate
	static void Reset();

public:
	static void Initialize();
	static void Tick();
	static double getTickCount();
	static Game_ID getNextID();

private:
	static BasicINIParser::BasicINIData INI_file; //TODO: should this be stored here?

public:
	static void initializeINI(std::string path);
	static const BasicINIParser::BasicINIData& get_INI();

private:
	GameManager() = delete;
	GameManager(const GameManager&) = delete;
};
