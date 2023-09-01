#include "winning-path-level.h"
#include "level-manager.h"
#include "constants.h"
#include "level-helper.h"
#include "tank-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "reset-things.h"
#include "rng.h"
#include <iostream>

ColorValueHolder WinningPathLevel::getDefaultColor() const {
	return ColorValueHolder(0.0f, 0.0f, 0.0f);
	//in JS, it just used the background rect's color
}

std::unordered_map<std::string, float> WinningPathLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", .25f });
	weights.insert({ "random-vanilla", .125f });
	weights.insert({ "old", .25f });
	weights.insert({ "random-old", .25f });
	return weights;
}

void WinningPathLevel::tick() {
	//nothing
}

void WinningPathLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40);

	ColorValueHolder color = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	//GenericFactoryConstructionData constructionData;
	//double* posArr;

	PositionHolder* wallArray = new PositionHolder[4];
	for (int i = 0; i < 4; i++) {
		//classic JS walls
		wallArray[i] = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(wallArray[i].x, wallArray[i].y, 32, 128, color));
	}

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "godmode"));

	pos = LevelHelper::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 20, 20, GAME_HEIGHT/2);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, GAME_HEIGHT/2, color));
	pos = LevelHelper::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 20, 20, GAME_HEIGHT/2);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, GAME_HEIGHT/2, color));

	const double WALL_WIDTH = 32;
	const double WALL_HEIGHT = 16;
	const int WALL_COUNT = 8;

	for (int i = 0; i < 4*WALL_COUNT; i++) {
		double w = RNG::randFunc() * (WALL_WIDTH - 8) + 8;
		double h = RNG::randFunc() * (WALL_HEIGHT - 4) + 4;
		//JS floored w and h (also, it didn't allow w=8 or h=4 for some reason)
		double x_offset = RNG::randFunc() * ((wallArray[3].x - 40) - (GAME_WIDTH/2 + 20+20) - w);
		double y_offset = RNG::randFunc() * ((GAME_HEIGHT/2 - 40) - (GAME_HEIGHT/4) - h);
		//space from LR walls' corner to classic walls, minus 40 for the tanks to move around
		pos = LevelHelper::getSymmetricWallPositions_Corners(i%4, GAME_WIDTH/2, GAME_HEIGHT/2, 20+20 + x_offset, GAME_HEIGHT/4 + y_offset, w, h);
		WallManager::pushWall(new Wall(pos.x, pos.y, w, h, color));
	}

	delete[] wallArray;
}

Level* WinningPathLevel::factory() {
	return new WinningPathLevel();
}

WinningPathLevel::WinningPathLevel() {
	GenericFactoryConstructionData constructionData;
	effects.push_back(LevelManager::getLevelEffectFactory("vanilla", "invisible_walls")(constructionData));
}
