#include "trickymaneuveringlevel.h"
#include "levelmanager.h"
#include "constants.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "powerupmanager.h"
#include "wallmanager.h"
#include "hazardmanager.h"
#include "mylib.h"
#include "resetthings.h"
#include <iostream>

std::unordered_map<std::string, float> TrickyManeuveringLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", .25f });
	weights.insert({ "random-vanilla", .125f });
	weights.insert({ "old", .125f });
	weights.insert({ "random-old", .125f });
	return weights;
}

void TrickyManeuveringLevel::tick() {
	//TODO: should this be changed?
	doLevelEffects();
}

void TrickyManeuveringLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40);

	//in JS, power mixing was turned off
	ColorValueHolder color = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	std::string* paras;
	std::string* names;

	PositionHolder* wallArray = new PositionHolder[4];
	for (int i = 0; i < 4; i++) {
		//classic JS walls
		wallArray[i] = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(wallArray[i].x, wallArray[i].y, 32, 128, color));
	}

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 40, 40, 20, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, 20, 20, color));
	}

	paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 40), std::to_string(GAME_HEIGHT/2 - 40 - 20), std::to_string(40*2), std::to_string(20)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "horizontal_lightning")(4, paras));
	delete[] paras;
	paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 40), std::to_string(GAME_HEIGHT/2 + 40), std::to_string(40*2), std::to_string(20)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "horizontal_lightning")(4, paras));
	delete[] paras;
	paras = new std::string[4]{std::to_string(GAME_WIDTH/2 - 40 - 20), std::to_string(GAME_HEIGHT/2 - 40), std::to_string(20), std::to_string(40*2)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(4, paras));
	delete[] paras;
	paras = new std::string[4]{std::to_string(GAME_WIDTH/2 + 40), std::to_string(GAME_HEIGHT/2 - 40), std::to_string(20), std::to_string(40*2)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(4, paras));
	delete[] paras;

	paras = new std::string[4]{std::to_string(wallArray[0].x+32), std::to_string(0), std::to_string(wallArray[3].x - (wallArray[0].x+32)), std::to_string(16)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lava")(4, paras));
	delete[] paras;
	paras = new std::string[4]{std::to_string(wallArray[0].x+32), std::to_string(GAME_HEIGHT - 16), std::to_string(wallArray[3].x - (wallArray[0].x+32)), std::to_string(16)};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lava")(4, paras));
	delete[] paras;

	//traps:
	//for (int i = 0; i < 4; i++) {
	//	pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 60-20/2, 60+20/2);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//}
	//for (int i = 0; i < 4; i++) {
	//	pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 60+20/2, 60-20/2);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//}

	//for (int i = 0; i < 4; i++) {
	//	pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 80+20/2, 80+20/2);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "banana"));
	//}
	//for (int i = 0; i < 4; i++) {
	//	pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 16/2, 16/2);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed")); //not trap
	//}

	//[insert LightningCorner's center powerup here] (JS has it in its comments)

	//names = new std::string[3]{ "big", "blast", "banana" }; //JS
	names = new std::string[3]{ "big", "banana", "banana" };
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, names, 3));
	delete[] names;
	//in JS, blast's bullet acceleration did not affect banana
	//TODO: power constructors take arguments, increase second banana's bananaCount to 16

	//did this level come first, or did LightningCorners? probably LightningCorners

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 16, 16);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "invincible"));
	}

	names = new std::string[2]{ "speed", "barrier" };
	paras = new std::string[2]{ "vanilla", "vanilla-extra" };
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, (wallArray[3].x - (wallArray[0].x+32))/2 - 16, GAME_HEIGHT/2 - (16+16));
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, paras, names, 2));
	}
	delete[] names;
	delete[] paras;

	//not here in JS:
	//pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, 80-20/2);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "dev", "inversion"));
	//pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, 80-20/2);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "dev", "inversion"));

	delete[] wallArray;
}

Level* TrickyManeuveringLevel::factory() {
	return new TrickyManeuveringLevel();
}

TrickyManeuveringLevel::TrickyManeuveringLevel() {
	std::string temp[1] = { "1" };
	effects.push_back(LevelManager::getLevelEffectFactory("vanilla", "wind")(1, temp));
}
