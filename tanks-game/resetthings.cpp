#include "resetthings.h"
#include "gamemanager.h"
#include "endgamehandler.h"
#include "constants.h"
#include "tankmanager.h"
#include "bulletmanager.h"
#include "wallmanager.h"
#include "powerupmanager.h"
#include "hazardmanager.h"
#include "levelmanager.h"
#include "mylib.h"
#include "rng.h"
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

	BulletManager::clearBullets();
	WallManager::clearWalls();
	PowerupManager::clearPowerups();
	HazardManager::clearCircleHazards();
	HazardManager::clearRectHazards();
	LevelManager::clearLevels();

	TankManager::tanks[0]->resetThings(default_tankToEdgeDist, GAME_HEIGHT/2, 0, TankManager::tanks[0]->getTeamID());
	TankManager::tanks[1]->resetThings(GAME_WIDTH - default_tankToEdgeDist, GAME_HEIGHT/2, PI, TankManager::tanks[1]->getTeamID());

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
