#include "reset-things.h"
#include "game-manager.h"
#include "end-game-handler.h"
#include "constants.h"
#include "tank-manager.h"
#include "bullet-manager.h"
#include "wall-manager.h"
#include "powerup-manager.h"
#include "hazard-manager.h"
#include "level-manager.h"
#include "mylib.h"
#include "rng.h"
#include "diagnostics.h"
#include <iostream>

const double ResetThings::default_tankToEdgeDist = 20;

void ResetThings::reset(int) {
	EndGameHandler::finalizeScores();
	//TODO: this good?
	for (int i = 0; i < EndGameHandler::teamsParticipating.size(); i++) {
		std::cout << EndGameHandler::teamsParticipating[i].teamName << " score: " << EndGameHandler::teamWins[EndGameHandler::teamsParticipating[i].teamID];

		if (i != EndGameHandler::teamsParticipating.size()-1) {
			std::cout << " | ";
		} else {
			std::cout << std::endl;
		}
	}

	TankInputChar* tankInput1 = TankManager::getTank(0)->getKeys();
	TankInputChar* tankInput2 = TankManager::getTank(1)->getKeys();
	//TODO: need to get inputs, name, and teamID; need something like Tank::getIdentification()
	//maybe it also needs to store starting angle (if getNumTanks() > 2)

	BulletManager::clearBullets();
	WallManager::clearWalls();
	PowerupManager::clearPowerups();
	HazardManager::clearCircleHazards();
	HazardManager::clearRectHazards();
	LevelManager::clearLevels();
	TankManager::clearTanks();

	TankManager::pushTank(new Tank(default_tankToEdgeDist, GAME_HEIGHT/2, 0, 1, "WASD", tankInput1));
	TankManager::pushTank(new Tank(GAME_WIDTH-default_tankToEdgeDist, GAME_HEIGHT/2, PI, 2, "Arrow Keys", tankInput2));
	delete[] tankInput1, tankInput2;

#if _DEBUG
	LevelManager::pushLevel("dev", "dev0");
#else
	std::vector<float> levelWeights;
	levelWeights.reserve(LevelManager::getNumLevelTypes("random-vanilla"));
	for (int i = 0; i < LevelManager::getNumLevelTypes("random-vanilla"); i++) {
		std::string n = LevelManager::getLevelName("random-vanilla", i);
		Level* l = LevelManager::getLevelFactory("random-vanilla", n)();
		levelWeights.push_back(l->getWeights()["random-vanilla"]);
		delete l;
	}
	int levelIndex = weightedSelect<float>(levelWeights.data(), levelWeights.size());
	std::string levelName = LevelManager::getLevelName("random-vanilla", levelIndex);
	LevelManager::pushLevel("random-vanilla", levelName);

	/*
	for (int i = 0; i < levelWeights.size(); i++) {
		std::cout << levelWeights[i] << ", ";
	}
	std::cout << levelIndex << ", " << levelName << std::endl;
	*/
#endif

	//initialize levels from LevelManager level list
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->initialize();
	}
	//initialize their level effects
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		Level* l = LevelManager::getLevel(i);
		for (int j = 0; j < l->getNumEffects(); j++) {
			l->getLevelEffect(j)->apply();
		}
	}

	GameManager::Reset();
	Diagnostics::clearGraph();
}

void ResetThings::firstReset() {
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->initialize();
	}
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		Level* l = LevelManager::getLevel(i);
		for (int j = 0; j < l->getNumEffects(); j++) {
			l->getLevelEffect(j)->apply();
		}
	}
}

//TODO: tankPositionReset should have a version with no Tank inputs
//worry about this when there's more than two tanks

void ResetThings::tankPositionReset(Tank* first, Tank* second) {
	tankPositionReset(first, second, default_tankToEdgeDist);
}

void ResetThings::tankPositionReset(Tank* first, Tank* second, double x) {
	x = constrain<double>(x, 0, GAME_WIDTH); //trolls begone
	first->x = x;
	second->x = GAME_WIDTH - x;

	int randNum = RNG::randFunc() * 5;
	first->y = randNum * (GAME_HEIGHT/5) + (GAME_HEIGHT/10);
	second->y = (4 - randNum) * (GAME_HEIGHT/5) + (GAME_HEIGHT/10);
}

void ResetThings::tankPositionReset(Tank* first, Tank* second, double x, double y) {
	x = constrain<double>(x, 0, GAME_WIDTH); //no trolls here
	y = constrain<double>(y, 0, GAME_HEIGHT); //trolls begone
	first->x = x;
	second->x = GAME_WIDTH - x;
	first->y = y;
	second->y = GAME_HEIGHT - y;
}
