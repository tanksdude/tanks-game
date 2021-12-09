#include "timedrewardlevel.h"
#include "constants.h"
#include <math.h>
#include "randomlevel.h"
#include "tankmanager.h"
#include "wallmanager.h"
#include "powerupmanager.h"
#include "hazardmanager.h"
#include "resetthings.h"
#include "gamemanager.h"

ColorValueHolder TimedRewardLevel::getDefaultColor() const {
	//return ColorValueHolder(0x97/255.0, 0x5C/255.0, 0x5C/255.0);
	//return ColorValueHolder(0x9E/255.0, 0x4C/255.0, 0x4C/255.0);
	return ColorValueHolder(0x97/255.0, 0x46/255.0, 0x46/255.0);
}

std::unordered_map<std::string, float> TimedRewardLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", .25f });
	weights.insert({ "random-vanilla", .125f });
	weights.insert({ "dev", .5f });
	weights.insert({ "random-dev", .5f });
	//shouldn't really be in a regular group since it's not done... but it's fine
	return weights;
}

void TimedRewardLevel::tick() {
	//the intention of the moving walls was not to move sinusoidally but more linearly (like a triangle wave but square wave tips)
	//I don't really care since this isn't an actual level right now; maybe it'll get updated in the future

	for (int i = 0; i < 4; i++) {
		Wall* tempWall = WallManager::getWallByID(movingWalls[i]);
		if (tempWall != nullptr) {
			//tempWall->y += (10 * sin(2*PI * GameManager::getTickCount() / 200)) * ((i/2 == 0)*2-1);
			tempWall->y += (2*PI/200 * 10 * cos(2*PI * GameManager::getTickCount() / 200)) * ((i/2 == 0)*2-1); //derivative
			//std::cout << tempWall->y << std::endl;
		}
	}
}

void TimedRewardLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40);

	ColorValueHolder wallColor = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	std::string* paras;

	pos = RandomLevel::getSymmetricWallPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, 40, 120*2, 40);
	WallManager::pushWall(new Wall(pos.x, pos.y, 120*2, 40, wallColor));
	pos = RandomLevel::getSymmetricWallPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, 40, 120*2, 40);
	WallManager::pushWall(new Wall(pos.x, pos.y, 120*2, 40, wallColor));

	//moving walls
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 120-20, 20/2, 20, 40+20);
		//Wall* tempWall = new Wall(pos.x, pos.y, 20, 40+20, ColorValueHolder(0, 0, 0));
		Wall* tempWall = new Wall(pos.x, pos.y, 20, 40+20, wallColor);
		movingWalls[i] = tempWall->getGameID();
		WallManager::pushWall(tempWall);
	}

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "godmode"));

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 120 - 20 - 4*2, 40 - 4*2);
		SimpleVector2D distToCenter = SimpleVector2D(GAME_WIDTH/2 - pos.x, GAME_HEIGHT/2 - pos.y);
		paras = new std::string[3]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(distToCenter.getAngle())};
		HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(3, paras));
		delete[] paras;
	}

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 120 + 60, GAME_HEIGHT/2 - (40+40)/2);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "shield"));
	}

	pos = RandomLevel::getSymmetricWallPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, 40+40, 20, (GAME_HEIGHT/2 - (40+40)));
	paras = new std::string[4]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(20), std::to_string(GAME_HEIGHT/2 - (40+40))};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(4, paras));
	delete[] paras;
	pos = RandomLevel::getSymmetricWallPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, 40+40, 20, (GAME_HEIGHT/2 - (40+40)));
	paras = new std::string[4]{std::to_string(pos.x), std::to_string(pos.y), std::to_string(20), std::to_string(GAME_HEIGHT/2 - (40+40))};
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(4, paras));
	delete[] paras;

	//other potential powers:
	//homing, tracking, big, speed?, triple?, blast, annoying
	//other potential hazards:
	//lava (moving)? targeting turret
}

Level* TimedRewardLevel::factory() {
	return new TimedRewardLevel();
}

TimedRewardLevel::TimedRewardLevel() {
	movingWalls = new Game_ID[4];
}

TimedRewardLevel::~TimedRewardLevel() {
	delete[] movingWalls;
}
