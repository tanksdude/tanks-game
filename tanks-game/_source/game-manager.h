#pragma once
#include "constants.h"
#include <vector>

#include "game-thing.h"
#include "basic-ini-parser.h"
#include "game-settings.h"

class GameManager {
	friend class DeveloperManager;
	friend class ResetThings;

private:
	static std::vector<GameThing*> objectsInGame;
	static size_t objectsInGame_previousSize;
	static void clearObjects(); //for ResetThings

public:
	static void initializeObjectList();
	static GameThing* getObject(unsigned int index);
	static GameThing* getObjectByID(Game_ID);
	static void pushObject(GameThing*);
	static unsigned int getNumObjects() { return objectsInGame.size(); }
	static void deleteObject(unsigned int index);
	static void deleteObjectByID(Game_ID);

	static void updateEveryAABB();
	static std::vector<GameThing*>& getObjectCollisionList() { return objectsInGame; } //for PhysicsHandler

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
	static void CreateDefaultINIFileIfNeeded(std::string path);
	static void initializeINI(std::string path);
	static void initializeSettings();
	static const BasicINIParser::BasicINIData& get_INI();
	static const GameSettings& get_settings();

private:
	GameManager() = delete;
	GameManager(const GameManager&) = delete;
};
