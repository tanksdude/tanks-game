#include "many-hazards-level.h"
#include "constants.h"
#include "random-level.h"
#include "tank-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "mylib.h"
#include "reset-things.h"
#include <iostream>

std::unordered_map<std::string, float> ManyHazardsLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "old", .5f });
	weights.insert({ "random-old", .5f });
	weights.insert({ "random", .5f });
	return weights;
}

void ManyHazardsLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40);

	//in JS, power mixing was turned off
	ColorValueHolder color = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;

	PositionHolder* wallArray = new PositionHolder[4];
	for (int i = 0; i < 4; i++) {
		//classic JS walls
		wallArray[i] = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(wallArray[i].x, wallArray[i].y, 32, 128, color));
	}

	posArr = new double[4]{ GAME_WIDTH/2 - 10, 128, 10*2, GAME_HEIGHT - 128*2 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(constructionData));
	delete[] posArr;

	const int TURRET_COUNT = 16; //stationary turret count, not regular turret count
	//const double turretDistance = (wallArray[3].x - (wallArray[0].x+32)) / (TURRET_COUNT+1);
	const double turretDistance = (wallArray[3].x - (wallArray[0].x+32) - 8) / (TURRET_COUNT+1);
	for (int i = 0; i < TURRET_COUNT; i++) {
		//bottom half
		//posArr = new double[3]{ wallArray[0].x+32 + turretDistance * (i+1), 16, PI/2 };
		posArr = new double[3]{ wallArray[0].x+32+8/2 + turretDistance * (i+1), 16, PI/2 };
		constructionData = GenericFactoryConstructionData(3, posArr);
		HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(constructionData));
		delete[] posArr;
	}
	for (int i = 0; i < TURRET_COUNT; i++) {
		//top half
		//posArr = new double[3]{ wallArray[0].x+32 + turretDistance * (i+1), GAME_HEIGHT-16, -PI/2 };
		posArr = new double[3]{ wallArray[0].x+32+8/2 + turretDistance * (i+1), GAME_HEIGHT-16, -PI/2 };
		constructionData = GenericFactoryConstructionData(3, posArr);
		HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "stationary_turret")(constructionData));
		delete[] posArr;
	}
	//JS x positions: [140, 164, ..., 476, 500]; C++ x positions: [136.471, 160.941, ..., 479.059, 503.529]
	//positions have been updated to JS version; old version left commented out

	//this is too much:
	//for (int i = 0; i < TURRET_COUNT-1; i++) {
	//	posArr = new double[3]{ wallArray[0].x+32+8/2 + turretDistance * (i+1) + 4+8, 16, PI/2 };
	//	constructionData = GenericFactoryConstructionData(3, posArr);
	//	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "targeting_turret")(constructionData));
	//	delete[] posArr;
	//}
	//for (int i = 0; i < TURRET_COUNT-1; i++) {
	//	posArr = new double[3]{ wallArray[0].x+32+8/2 + turretDistance * (i+1) + 4+8, GAME_HEIGHT-16, -PI/2 };
	//	constructionData = GenericFactoryConstructionData(3, posArr);
	//	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "targeting_turret")(constructionData));
	//	delete[] posArr;
	//}

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - ((wallArray[0].x+32) + (TANK_RADIUS/2 * 1.5)), 32 + 20 + (TANK_RADIUS/2 * 2.5));
		//pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - ((wallArray[0].x+32) + (TANK_RADIUS/2 * 1.5) + 4), 32 + 20 + (TANK_RADIUS/2 * 2.5));
		posArr = new double[3]{ pos.x, pos.y, PI * (i%2) };
		constructionData = GenericFactoryConstructionData(3, posArr);
		HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "targeting_turret")(constructionData));
		delete[] posArr;
	}

	//useless traps:
	//for (int i = 0; i < 4; i++) {
	//	pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - (wallArray[0].x+32 + 20), GAME_HEIGHT/2 - 48);
	//	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//}

	PositionHolder wallPos1 = RandomLevel::getSymmetricWallPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT - 128*2)/2, 160, 20);
	WallManager::pushWall(new Wall(wallPos1.x, wallPos1.y, 160, 20, color));
	PositionHolder wallPos2 = RandomLevel::getSymmetricWallPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT - 128*2)/2, 160, 20);
	WallManager::pushWall(new Wall(wallPos2.x, wallPos2.y, 160, 20, color));

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 160/2, (GAME_HEIGHT - 128*2)/2, (GAME_WIDTH/2 - 160/2 - (wallArray[0].x+32)), 20);
		posArr = new double[4]{ pos.x, pos.y, GAME_WIDTH/2 - 160/2 - (wallArray[0].x+32), 20 };
		constructionData = GenericFactoryConstructionData(4, posArr);
		HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "horizontal_lightning")(constructionData));
		delete[] posArr;
	}

	pos = RandomLevel::getSymmetricPowerupPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 160/4 * 1.5);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "barrier")); //replace barrier with speed?
	pos = RandomLevel::getSymmetricPowerupPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 160/4 * 1.5);
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "barrier"));

	pos = RandomLevel::getSymmetricWallPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2 - wallPos1.y, 160/2, 50);
	posArr = new double[4]{ pos.x, pos.y, 160/2, 50 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lava")(constructionData));
	delete[] posArr;
	pos = RandomLevel::getSymmetricWallPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2 - wallPos1.y, 160/2, 50);
	posArr = new double[4]{ pos.x, pos.y, 160/2, 50 };
	constructionData = GenericFactoryConstructionData(4, posArr);
	HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lava")(constructionData));
	delete[] posArr;

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 160/4, GAME_HEIGHT/2 - wallPos1.y, 160/4, 50);
		posArr = new double[4]{ pos.x, pos.y, 160/4, 50 };
		constructionData = GenericFactoryConstructionData(4, posArr);
		HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "lightning")(constructionData));
		delete[] posArr;
	}

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 160/4 * 1.5, (GAME_HEIGHT - 128*2)/2 + 20 + 50 + 21);
		//(160 - 32 - 20 - 50 - 16)/2 = 21
		//(GAME_HEIGHT/2 - (GAME_HEIGHT - 128*2)/2 - [wall height] - [lava height] - [stationary turret distance to edge])/2
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "invincible"));
	}

	//traps:
	//pos = RandomLevel::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT - 128*2)/2 + 20 + 50 + 21);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//pos = RandomLevel::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT - 128*2)/2 + 20 + 50 + 21);
	//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	//yes, they were evil

	delete[] wallArray;
}

Level* ManyHazardsLevel::factory() {
	return new ManyHazardsLevel();
}

ManyHazardsLevel::ManyHazardsLevel() { return; }
