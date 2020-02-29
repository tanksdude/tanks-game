#pragma once
#include "level.h"
#include <vector>

class LevelManager {
	friend class ResetThings;
private:
	static std::vector<Level*> levels; //active levels
	//static std::vector<LevelEffect*> leveleffects; //miscellaneous level effects //would this ever get used? aren't all level effects supposed to be attached to levels? (note: multiple levels can use the same level effect)
public:
	static void initialize();
	static Level* const getLevel(int index); //why would this be needed
	static LevelEffect* const getLevelEffect(int level_index, int index); //why would this be needed, like, ever?
	//see BulletManager explanation
	static void pushLevel(Level*);
	//static void pushLevelEffect(LevelEffect*);
	static int getNumLevels() { return levels.size(); }
	static int getLevel_numEffects(int index) { return levels[index]->effects.size(); }
	//static int getNumLevelEffects(); //total level effects //hopefully not confusing //why would this be used?
	//static void deleteLevel(int index); //only way to remove a level is to start a new game, handled by ResetThings
	static void clearLevels(); //in case ResetThings shouldn't do any resetting
	//static void deleteLevelEffect(int index); //this doesn't make sense
};