#include "resetthings.h"
#include "constants.h"
#include "tankmanager.h"
#include "bulletmanager.h"
#include "wallmanager.h"
#include "powerupmanager.h"
#include "hazardmanager.h"
#include "levelmanager.h"
#include "mylib.h"

void ResetThings::reset(int) {
	TankManager::tanks[0]->resetThings(20, GAME_HEIGHT/2, 0, TankManager::tanks[0]->getTeamID(), TankManager::tanks[0]->getName());
	TankManager::tanks[1]->resetThings(GAME_WIDTH - 20, GAME_HEIGHT/2, PI, TankManager::tanks[1]->getTeamID(), TankManager::tanks[1]->getName());

	BulletManager::clearBullets();
	WallManager::clearWalls();
	PowerupManager::clearPowerups();
	HazardManager::clearCircleHazards();
	HazardManager::clearRectHazards();
	LevelManager::clearLevels();

#if _DEBUG
	LevelManager::pushSpecialLevel("dev", "dev0");
#else
	int randLevel = randFunc() * LevelManager::getNumLevelTypes();
	std::string levelName = LevelManager::getLevelName(randLevel);
	if (levelName != "default random" || levelName == "empty") {
		randLevel = randFunc() * LevelManager::getNumLevelTypes();
		std::string levelName = LevelManager::getLevelName(randLevel);
		if (levelName == "empty") {
			randLevel = randFunc() * LevelManager::getNumLevelTypes();
			std::string levelName = LevelManager::getLevelName(randLevel);
		}
	}
	LevelManager::pushLevel(levelName);
#endif

	//initialize levels from LevelManager level list
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::levels[i]->initialize();
	}
}

void ResetThings::firstReset() {
	LevelManager::levels[0]->initialize();
}

void ResetThings::tankPositionReset(Tank* first, Tank* second, int randNum) {
	//x-position set by ResetThings (first line of reset(int))
	randNum = constrain<int>(randNum, 0, 4); //no trolls here
	first->y = randNum * (GAME_HEIGHT/5) + (GAME_HEIGHT/10);
	second->y = (4 - randNum) * (GAME_HEIGHT/5) + (GAME_HEIGHT/10);
}

void ResetThings::tankPositionReset(Tank* first, Tank* second, double x, int randNum) {
	x = constrain<double>(x, 0, GAME_WIDTH); //trolls begone
	first->x = x;
	second->x = GAME_WIDTH - x;

	randNum = constrain<int>(randNum, 0, 4); //trolls, no trolling
	first->y = randNum * (GAME_HEIGHT/5) + (GAME_HEIGHT/10);
	second->y = (4 - randNum) * (GAME_HEIGHT/5) + (GAME_HEIGHT/10);
}

void ResetThings::tankPositionReset(Tank* first, Tank* second, double x, double y, bool) {
	x = constrain<double>(x, 0, GAME_WIDTH); //troll police, open up!
	y = constrain<double>(y, 0, GAME_HEIGHT); //trolly mctrollface
	first->x = x;
	second->x = GAME_WIDTH - x;
	first->y = y;
	second->y = GAME_HEIGHT - y;
}
