#include "dangerous-center-level.h"
#include "constants.h"
#include "level-helper.h"
#include "tank-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "hazard-manager.h"
#include "reset-things.h"
#include "rng.h"
#include "level-manager.h"
#include <iostream>
#include "respawning-powerups-level-effect.h"

ColorValueHolder DangerousCenterLevel::getDefaultColor() const {
	//return ColorValueHolder(0xE6/255.0, 0xE6/255.0, 0xFA/255.0); //lavender
	return ColorValueHolder(0xE6/255.0, 0x91/255.0, 0xA9/255.0);
}

std::unordered_map<std::string, float> DangerousCenterLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", .25f });
	weights.insert({ "random-vanilla", .5f }); //should this even appear?
	return weights;
}

void DangerousCenterLevel::initialize() {
	//ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40);
	int randNum = RNG::randFunc() * 3;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), ResetThings::default_tankToEdgeDist, (randNum+1) * (GAME_HEIGHT/5) + (GAME_HEIGHT/10));
	//from tight patrolling corridor level

	ColorValueHolder color = getDefaultColor();
	int tempRand;
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	std::string* paras;
	std::string* names;
	PowerSquare* p;

	LevelEffect* le = nullptr;
	for (int i = 0; i < effects.size(); i++) {
		if (effects[i]->getName() == "respawning_powerups") {
			le = effects[i];
			break;
		}
	}
	if (le == nullptr) {
		throw std::logic_error("ERROR: \"dangerous_center\" level does not have \"respawning_powerups\" level effect!");
	}
	RespawningPowerupsLevelEffect* respawning = (RespawningPowerupsLevelEffect*)le;
	//TODO: should this be the preferred way of getting specific level effects?

	posArr = new double[3]{ GAME_WIDTH/2, GAME_HEIGHT/2, PI/2 * (floor(RNG::randFunc()*2)*2-1) };
	int* childArr = new int[2]{ 16, 8 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(2, childArr);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "mother_turret")(constructionData));
	delete[] posArr; delete[] childArr;

	//hiding areas from lone turret level, pushed out
	const double top_gap = TANK_RADIUS*2;
	const double side_length = 60;
	/*
	//equal x and y offsets from center with gap from top
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(top_gap+side_length), GAME_HEIGHT/2-(top_gap+side_length), side_length, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, side_length, 20, color));
	}
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(top_gap+side_length), GAME_HEIGHT/2-(top_gap+side_length), 20, side_length);
		WallManager::pushWall(new Wall(pos.x, pos.y, 20, side_length, color));
	}
	//x offset from center equals y offset from edge
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(top_gap+side_length), GAME_HEIGHT/2-(top_gap+side_length), side_length, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, side_length, 20, color));
	}
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(top_gap+side_length), GAME_HEIGHT/2-(top_gap+side_length), 20, side_length);
		WallManager::pushWall(new Wall(pos.x, pos.y, 20, side_length, color));
	}
	//middle of the two
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_WIDTH/2+GAME_HEIGHT/2)/2-(top_gap+side_length), GAME_HEIGHT/2-(top_gap+side_length), side_length, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, side_length, 20, color));
	}
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_WIDTH/2+GAME_HEIGHT/2)/2-(top_gap+side_length), GAME_HEIGHT/2-(top_gap+side_length), 20, side_length);
		WallManager::pushWall(new Wall(pos.x, pos.y, 20, side_length, color));
	}
	*/
	//just use a custom distance
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT/2+40)-(top_gap+side_length)+20, GAME_HEIGHT/2-(top_gap+side_length), side_length-20, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, side_length-20, 20, color));
	}
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT/2+40)-(top_gap+side_length), GAME_HEIGHT/2-(top_gap+side_length), 20, side_length);
		WallManager::pushWall(new Wall(pos.x, pos.y, 20, side_length, color));
	}

	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT/2+40)-(top_gap+side_length), GAME_HEIGHT/2-(top_gap), 20, top_gap);
		posArr = new double[4]{ pos.x, pos.y, 20, top_gap };
		constructionData = GenericFactoryConstructionData(4, posArr);
		HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "vertical_lightning")(constructionData));
		delete[] posArr;
	}

	//top/bottom walls
	pos = LevelHelper::getSymmetricWallPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(top_gap+10), 60, 10);
	WallManager::pushWall(new Wall(pos.x, pos.y, 60, 10, color));
	pos = LevelHelper::getSymmetricWallPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(top_gap+10), 60, 10);
	WallManager::pushWall(new Wall(pos.x, pos.y, 60, 10, color));
	//little hooks
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 60/2-10, GAME_HEIGHT/2-(top_gap+10)-(10), 10, 10);
		WallManager::pushWall(new Wall(pos.x, pos.y, 10, 10, color));
	}

	//starting walls
	pos = LevelHelper::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20), 20, GAME_HEIGHT-(top_gap+side_length-20)*2);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, GAME_HEIGHT-(top_gap+side_length-20)*2, color));
	pos = LevelHelper::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20), 20, GAME_HEIGHT-(top_gap+side_length-20)*2);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, GAME_HEIGHT-(top_gap+side_length-20)*2, color));

	//pocket for a power
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20)-(40), 20, 40, 10);
		WallManager::pushWall(new Wall(pos.x, pos.y, 40, 10, color));
	}

	//pocket powers:
	pos = LevelHelper::getSymmetricPowerupPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20)-(40)/2);
	p = new PowerSquare(pos.x, pos.y, "vanilla", "big");
	PowerupManager::pushPowerup(p);
	respawning->watchPowerSquare(p);
	pos = LevelHelper::getSymmetricPowerupPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-(40+20)-(40)/2);
	p = new PowerSquare(pos.x, pos.y, "vanilla", "big");
	PowerupManager::pushPowerup(p);
	respawning->watchPowerSquare(p);

	//top/bottom powers
	names = new std::string[2]{ "multishot", "bounce" };
	pos = LevelHelper::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(top_gap+10)-(10));
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, names, 2));
	pos = LevelHelper::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2-(top_gap+10)-(10));
	PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, names, 2));
	delete[] names;

	//concealed powers:
	paras = new std::string[2]{ "vanilla", "vanilla" };
	names = new std::string[2]{ "grenade", "triple" };
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT/2+40)-(top_gap+side_length)+(side_length/2), GAME_HEIGHT/2-(top_gap+side_length)+(side_length/2));
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, paras, names, 2));
		//TODO: should these be switched with the invincible powers?
	}
	delete[] paras; delete[] names;

	//not concealed powers:
	//paras = new std::string[2]{ "vanilla", "vanilla" };
	//names = new std::string[2]{ "fire", "invincible" };
	tempRand = RNG::randFunc() * 2;
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, (GAME_HEIGHT/2+40)-(top_gap+side_length)-(10), GAME_HEIGHT/2-(top_gap+side_length)-(10));
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla", LevelHelper::powerAlternate(i, tempRand, "fire", "invincible")));
	}
	//delete[] paras; delete[] names;
}

Level* DangerousCenterLevel::factory() {
	return new DangerousCenterLevel();
}

DangerousCenterLevel::DangerousCenterLevel() {
	bool temp[1] = { false };
	GenericFactoryConstructionData constructionData(1, temp);
	effects.push_back(LevelManager::getLevelEffectFactory("vanilla", "respawning_powerups")(constructionData));
}
