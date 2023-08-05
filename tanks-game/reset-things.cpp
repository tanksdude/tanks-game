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

	BulletManager::clearBullets();
	WallManager::clearWalls();
	PowerupManager::clearPowerups();
	HazardManager::clearCircleHazards();
	HazardManager::clearRectHazards();
	LevelManager::clearLevels();
	TankManager::clearTanks();

	//TODO: maybe GameMainLoop should hold the shooting cooldown? (but the tank still needs to get initialized with it...)
	const BasicINIParser::BasicINIData& ini_data = GameManager::get_INI();
	double shootCooldown = SHOOT_COOLDOWN;
	if (ini_data.exists("GAME_OPTIONS", "ShootingCooldown")) {
		shootCooldown = std::stod(ini_data.get("GAME_OPTIONS", "ShootingCooldown"));
	}

	TankManager::pushTank(new Tank(default_tankToEdgeDist, GAME_HEIGHT/2, 0, 1, "WASD", shootCooldown));
	TankManager::pushTank(new Tank(GAME_WIDTH-default_tankToEdgeDist, GAME_HEIGHT/2, PI, 2, "Arrow Keys", shootCooldown));

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

void ResetThings::firstGameInitialize(std::string tank1TeamName, std::string tank2TeamName, GameMainLoop& gameInstance) {
	const BasicINIParser::BasicINIData& ini_data = GameManager::get_INI();

	std::string tank1Name = "WASD";
	std::string tank2Name = "Arrow Keys";
	if (ini_data.exists("CONTROLS", "Tank1.Name")) {
		tank1Name = ini_data.get("CONTROLS", "Tank1.Name");
	}
	if (ini_data.exists("CONTROLS", "Tank2.Name")) {
		tank2Name = ini_data.get("CONTROLS", "Tank2.Name");
	}
	if (ini_data.exists("CONTROLS", "Tank1.TeamName")) {
		tank1TeamName = ini_data.get("CONTROLS", "Tank1.TeamName");
	}
	if (ini_data.exists("CONTROLS", "Tank2.TeamName")) {
		tank2TeamName = ini_data.get("CONTROLS", "Tank2.TeamName");
	}

	std::string tank1Forward = "w", tank1Left = "a", tank1Right = "d", tank1Shoot = "s", tank1Special = "e";
	if (ini_data.exists("CONTROLS", "Tank1.Forward")) {
		tank1Forward = ini_data.get("CONTROLS", "Tank1.Forward");
	}
	if (ini_data.exists("CONTROLS", "Tank1.Left")) {
		tank1Left = ini_data.get("CONTROLS", "Tank1.Left");
	}
	if (ini_data.exists("CONTROLS", "Tank1.Right")) {
		tank1Right = ini_data.get("CONTROLS", "Tank1.Right");
	}
	if (ini_data.exists("CONTROLS", "Tank1.Shoot")) {
		tank1Shoot = ini_data.get("CONTROLS", "Tank1.Shoot");
	}
	if (ini_data.exists("CONTROLS", "Tank1.Special")) {
		tank1Special = ini_data.get("CONTROLS", "Tank1.Special");
	}
	std::string tank2Forward = "Up", tank2Left = "Left", tank2Right = "Right", tank2Shoot = "Down", tank2Special = "/";
	if (ini_data.exists("CONTROLS", "Tank2.Forward")) {
		tank2Forward = ini_data.get("CONTROLS", "Tank2.Forward");
	}
	if (ini_data.exists("CONTROLS", "Tank2.Left")) {
		tank2Left = ini_data.get("CONTROLS", "Tank2.Left");
	}
	if (ini_data.exists("CONTROLS", "Tank2.Right")) {
		tank2Right = ini_data.get("CONTROLS", "Tank2.Right");
	}
	if (ini_data.exists("CONTROLS", "Tank2.Shoot")) {
		tank2Shoot = ini_data.get("CONTROLS", "Tank2.Shoot");
	}
	if (ini_data.exists("CONTROLS", "Tank2.Special")) {
		tank2Special = ini_data.get("CONTROLS", "Tank2.Special");
	}

	gameInstance.tank1Inputs[0] = TankInputChar(tank1Forward);
	gameInstance.tank1Inputs[1] = TankInputChar(tank1Left);
	gameInstance.tank1Inputs[2] = TankInputChar(tank1Right);
	gameInstance.tank1Inputs[3] = TankInputChar(tank1Shoot);
	gameInstance.tank1Inputs[4] = TankInputChar(tank1Special);
	gameInstance.tank2Inputs[0] = TankInputChar(tank2Forward);
	gameInstance.tank2Inputs[1] = TankInputChar(tank2Left);
	gameInstance.tank2Inputs[2] = TankInputChar(tank2Right);
	gameInstance.tank2Inputs[3] = TankInputChar(tank2Shoot);
	gameInstance.tank2Inputs[4] = TankInputChar(tank2Special);

	double shootCooldown = SHOOT_COOLDOWN;
	if (ini_data.exists("GAME_OPTIONS", "ShootingCooldown")) {
		shootCooldown = std::stod(ini_data.get("GAME_OPTIONS", "ShootingCooldown"));
	}

	TankManager::pushTank(new Tank(20, GAME_HEIGHT/2, 0, 1, tank1Name, shootCooldown));
	TankManager::pushTank(new Tank(GAME_WIDTH-20, GAME_HEIGHT/2, PI, 2, tank2Name, shootCooldown));
	EndGameHandler::addTeamToWatch(1, tank1TeamName);
	EndGameHandler::addTeamToWatch(2, tank2TeamName);
	EndGameHandler::addTeamToWatch(HAZARD_TEAM, "HAZARDS");
}

void ResetThings::firstLevelPush() {
	const BasicINIParser::BasicINIData& ini_data = GameManager::get_INI();

	if (ini_data.exists("GAME_OPTIONS", "GameFirstLevel") && (ini_data.length("GAME_OPTIONS", "GameFirstLevel") >= 2)) {
		LevelManager::pushLevel(ini_data.get("GAME_OPTIONS", "GameFirstLevel", 0), ini_data.get("GAME_OPTIONS", "GameFirstLevel", 1));
	} else {
		LevelManager::pushLevel("vanilla", "default_random");
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
