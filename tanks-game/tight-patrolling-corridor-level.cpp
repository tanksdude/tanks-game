#include "tight-patrolling-corridor-level.h"
#include "constants.h"
#include "random-level.h"
#include "tank-manager.h"
#include "wall-manager.h"
#include "powerup-manager.h"
#include "hazard-manager.h"
#include "reset-things.h"
#include "rng.h"

ColorValueHolder TightPatrollingCorridorLevel::getDefaultColor() const {
	//return ColorValueHolder(15.0f/16, 15.0f/16, 12.0f/16); //very icky yellow (at least it's unique; it just looks a little disgusting)
	return ColorValueHolder(15.0f/16, 15.0f/16, 14.0f/16); //very white beige
	//the color idea was beige but very white 'cuz that's vaguely like the white hallways in spy movies
	//(is that a valid idea? I'm not really sure how spy movies should look)
}

std::unordered_map<std::string, float> TightPatrollingCorridorLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 0.5f });
	weights.insert({ "random-vanilla", 0.25f });
	//TODO: update the weights eventually
	return weights;
}

void TightPatrollingCorridorLevel::initialize() {
	int randNum = RNG::randFunc() * 3;
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), (GAME_WIDTH/2 - ((60+20 + 40) + 80 + 20))/2, (randNum+1) * (GAME_HEIGHT/5) + (GAME_HEIGHT/10));
	//starting x determined by outer walls (see below)

	ColorValueHolder color = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	std::string* names;
	int* patrolCount;
	double* patrolRoute1;
	double* patrolRoute2;

	/*
	PositionHolder* wallArray = new PositionHolder[4];
	for (int i = 0; i < 4; i++) {
		//classic JS walls
		wallArray[i] = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(wallArray[i].x, wallArray[i].y, 32, 128, color));
	}
	*/

	posArr = new double[3]{ GAME_WIDTH/2 - 40, GAME_HEIGHT/2, 0 };
	patrolCount = new int[1]{ 2 };
	patrolRoute1 = new double[2*2]{
		GAME_WIDTH/2 - 40, GAME_HEIGHT/2 + 40,
		GAME_WIDTH/2 - 40, GAME_HEIGHT/2 - 40
	};
	patrolRoute2 = new double[2*2]{
		GAME_WIDTH/2 + 40, GAME_HEIGHT/2 - 40,
		GAME_WIDTH/2 + 40, GAME_HEIGHT/2 + 40
	};
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(2*2, patrolRoute1);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "patrolling_turret")(constructionData));
	delete[] posArr;
	posArr = new double[3]{ GAME_WIDTH/2 + 40, GAME_HEIGHT/2, PI };
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(2*2, patrolRoute2);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "patrolling_turret")(constructionData));
	delete[] posArr, patrolCount, patrolRoute1, patrolRoute2;

	//inner walls
	pos = RandomLevel::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 60, 20, (40+40)*2);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, (40+40)*2, color));
	pos = RandomLevel::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 60, 20, (40+40)*2);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, (40+40)*2, color));

	//PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "godmode"));
	names = new std::string[4]{ "bounce", "homing", "banana", "banana" };
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, names, 4));
	delete[] names;

	//"inner" powers
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 60+20+20/2, 40+40/2);
		//PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "shield"));
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "barrier"));
	}

	//next layer
	//pos = RandomLevel::getSymmetricWallPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, (40+30) + 40, (60+20/2 + 30)*2, 20);
	pos = RandomLevel::getSymmetricWallPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2 - 20, (60+20 + 40)*2, 20);
	WallManager::pushWall(new Wall(pos.x, pos.y, (60+20 + 40)*2, 20, color));
	pos = RandomLevel::getSymmetricWallPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2 - 20, (60+20 + 40)*2, 20);
	WallManager::pushWall(new Wall(pos.x, pos.y, (60+20 + 40)*2, 20, color));

	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, (60+20 + 40) + 20 + 20/2, 40-10, 20/2, 40+10*2);
		WallManager::pushWall(new Wall(pos.x, pos.y, 20/2, 40+10*2, color));
		//posArr = new double[4]{ pos.x, pos.y, 20/2, 40+10*2 };
		//constructionData = GenericFactoryConstructionData(4, posArr);
		//HazardManager::pushRectHazard(HazardManager::getRectHazardFactory("vanilla", "no_bullet_zone")(constructionData));
		//delete[] posArr;
	}

	posArr = new double[3]{ GAME_WIDTH/2, GAME_HEIGHT - 35, -PI/2 };
	patrolCount = new int[1]{ 2 };
	patrolRoute1 = new double[2*2]{
		GAME_WIDTH/2 - (60+20/2), GAME_HEIGHT - 35,
		GAME_WIDTH/2 + (60+20/2), GAME_HEIGHT - 35
	};
	patrolRoute2 = new double[2*2]{
		GAME_WIDTH/2 + (60+20/2), 35,
		GAME_WIDTH/2 - (60+20/2), 35
	};
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(2*2, patrolRoute1);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "patrolling_turret")(constructionData));
	delete[] posArr;
	posArr = new double[3]{ GAME_WIDTH/2, 35, PI/2 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(2*2, patrolRoute2);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "patrolling_turret")(constructionData));
	delete[] posArr, patrolCount, patrolRoute1, patrolRoute2;

	//outer walls
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, (60+20 + 40) + 80, 20, 20, GAME_HEIGHT/2-(20+40));
		WallManager::pushWall(new Wall(pos.x, pos.y, 20, GAME_HEIGHT/2-(20+40), color));
	}
	//extra
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, (60+20 + 40) + 80 + 20, GAME_HEIGHT/2-(20+40), GAME_WIDTH/2 - ((60+20 + 40) + 80 + 20), 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, GAME_WIDTH/2 - ((60+20 + 40) + 80 + 20), 20, color));
	}

	//extra powers
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - 20, GAME_HEIGHT/2 - 20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	}
	//experimental power positions
	for (int i = 0; i < 4; i++) {
		pos = RandomLevel::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - 10, GAME_HEIGHT/2 - (20+20+20+10));
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "invincible"));
	}

	//UL and UR turrets
	posArr = new double[3]{ GAME_WIDTH/2 - ((60+20 + 40) + 80 - 20), ((GAME_HEIGHT - 20) + (GAME_HEIGHT/2 + 20+40))/2, 0 };
	patrolCount = new int[1]{ 2 };
	patrolRoute1 = new double[2*2]{
		GAME_WIDTH/2 - ((60+20 + 40) + 80 - 20), GAME_HEIGHT - 20,
		GAME_WIDTH/2 - ((60+20 + 40) + 80 - 20), GAME_HEIGHT/2 + 20+40
	};
	patrolRoute2 = new double[2*2]{
		GAME_WIDTH/2 + ((60+20 + 40) + 80 - 20), GAME_HEIGHT - 20,
		GAME_WIDTH/2 + ((60+20 + 40) + 80 - 20), GAME_HEIGHT/2 + 20+40
	};
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(2*2, patrolRoute1);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "patrolling_turret")(constructionData));
	delete[] posArr;
	posArr = new double[3]{ GAME_WIDTH/2 + ((60+20 + 40) + 80 - 20), ((GAME_HEIGHT - 20) + (GAME_HEIGHT/2 + 20+40))/2, PI };
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(2*2, patrolRoute2);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "patrolling_turret")(constructionData));
	delete[] posArr, patrolCount, patrolRoute1, patrolRoute2;
	//DL and DR turrets
	posArr = new double[3]{ GAME_WIDTH/2 - ((60+20 + 40) + 80 - 20), ((20) + (GAME_HEIGHT/2 - (20+40)))/2, 0 };
	patrolCount = new int[1]{ 2 };
	patrolRoute1 = new double[2*2]{
		GAME_WIDTH/2 - ((60+20 + 40) + 80 - 20), 20,
		GAME_WIDTH/2 - ((60+20 + 40) + 80 - 20), GAME_HEIGHT/2 - (20+40)
	};
	patrolRoute2 = new double[2*2]{
		GAME_WIDTH/2 + ((60+20 + 40) + 80 - 20), 20,
		GAME_WIDTH/2 + ((60+20 + 40) + 80 - 20), GAME_HEIGHT/2 - (20+40)
	};
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(2*2, patrolRoute1);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "patrolling_turret")(constructionData));
	delete[] posArr;
	posArr = new double[3]{ GAME_WIDTH/2 + ((60+20 + 40) + 80 - 20), ((20) + (GAME_HEIGHT/2 - (20+40)))/2, PI };
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(1, patrolCount);
	constructionData.pushData(2*2, patrolRoute2);
	HazardManager::pushCircleHazard(HazardManager::getCircleHazardFactory("vanilla", "patrolling_turret")(constructionData));
	delete[] posArr, patrolCount, patrolRoute1, patrolRoute2;

	//delete[] wallArray;

	//level idea: N++ X-04-4 (pretty sure there are other levels with similar designs)
}

Level* TightPatrollingCorridorLevel::factory() {
	return new TightPatrollingCorridorLevel();
}

TightPatrollingCorridorLevel::TightPatrollingCorridorLevel() { return; }
