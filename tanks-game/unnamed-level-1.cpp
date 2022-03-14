#include "unnamed-level-1.h"
#include "constants.h"
#include "level-helper.h"
#include "tank-manager.h"
#include "wall-manager.h"
#include "powerup-manager.h"
#include "hazard-manager.h"
#include "reset-things.h"

ColorValueHolder UnnamedLevel1::getDefaultColor() const {
	//TODO
	return ColorValueHolder(1, 1, 1);
}

std::unordered_map<std::string, float> UnnamedLevel1::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.0f });
	weights.insert({ "random-dev", 0.0f });
	//still testing; once it's named it gets weights
	return weights;
}

void UnnamedLevel1::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1));

	ColorValueHolder color = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	//GenericFactoryConstructionData constructionData;
	//double* posArr;

	//outer walls
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 20, 20+20+40, 160, 20);
		WallManager::pushWall(new Wall(pos.x, pos.y, 160, 20, color));
	}
	pos = LevelHelper::getSymmetricWallPositions_LR(0, GAME_WIDTH/2, GAME_HEIGHT/2, 20+160-20, 20, (20+20+40)*2);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, (20+20+40)*2, color));
	pos = LevelHelper::getSymmetricWallPositions_LR(1, GAME_WIDTH/2, GAME_HEIGHT/2, 20+160-20, 20, (20+20+40)*2);
	WallManager::pushWall(new Wall(pos.x, pos.y, 20, (20+20+40)*2, color));

	//inner walls
	pos = LevelHelper::getSymmetricWallPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, 20, (20+160-20-40)*2, 20);
	WallManager::pushWall(new Wall(pos.x, pos.y, (20+160-20-40)*2, 20, color));
	pos = LevelHelper::getSymmetricWallPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, 20, (20+160-20-40)*2, 20);
	WallManager::pushWall(new Wall(pos.x, pos.y, (20+160-20-40)*2, 20, color));

	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 20+160+40, (GAME_HEIGHT/2 - (20+20+40+20))/2 + (20+20+40+20));
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "multishot"));
	}

	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 20+160-20-20, 20+20+20);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "speed"));
	}

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "vanilla-extra", "tracking"));

	//this level is basically a combination of hiding places, corridor level, and winning path...

	//reminds me of the N++ level X-19-4 (rocket chasing you, no walls, only platforms) or X-18-3 (must let rocket chase you)
	//potential idea: X-06-2 (run towards that gold at the end, run back fast enough to avoid rocket)

	//other potential powers:
	//
	//other potential hazards:
	//
}

Level* UnnamedLevel1::factory() {
	return new UnnamedLevel1();
}

UnnamedLevel1::UnnamedLevel1() { return; }
