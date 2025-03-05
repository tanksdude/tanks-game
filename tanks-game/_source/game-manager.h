#pragma once
#include "constants.h"

#include "basic-ini-parser.h"
#include "game-settings.h"

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
	static GameSettings settings_file;

public:
	static void initializeINI(std::string path);
	static void initializeSettings();
	static const BasicINIParser::BasicINIData& get_INI();
	static const GameSettings& get_settings();

private:
	GameManager() = delete;
	GameManager(const GameManager&) = delete;
};
