#include "mine-heaven-level.h"

#include "../constants.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"
#include "../level-manager.h"

std::unordered_map<std::string, float> MineHeavenLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", .25f });
	weights.insert({ "random-vanilla", .125f }); //people just do not like ice physics
	weights.insert({ "old", .25f });
	weights.insert({ "random-old", .125f });
	return weights;
}

void MineHeavenLevel::tick() {
	doLevelEffects();
}

void MineHeavenLevel::initialize() {
	ResetThings::tankPositionReset(40);

	//in JS, power mixing was turned off
	ColorValueHolder color = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	//GenericFactoryConstructionData constructionData;
	//double* posArr;
	std::string* paras;
	std::string* names;

	LevelHelper::pushClassicWalls(color);

	//total of 77 mine powerups

	paras = new std::string[2]{ "mines", "old_mines" };
	names = new std::string[2]{ "vanilla-extra", "old" };
	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, names, paras, 2));
	delete[] paras;
	delete[] names;
	//the reason there were two was to increase shooting speed

	const double mineSpacing = 16;
	const double horzDistFromEdge = 40*2 + 32 + mineSpacing; //= 128

	//8*4 on the diagonals from the center
	for (int i = 1; i <= 8; i++) {
		for (int j = 0; j < 4; j++) {
			pos = LevelHelper::getSymmetricPowerupPositions_Corners(j, GAME_WIDTH/2, GAME_HEIGHT/2, i * (GAME_WIDTH/2 - horzDistFromEdge)/8.0, i * (GAME_HEIGHT/2 - mineSpacing)/8.0);
			PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "mines"));
		}
	}

	//15*2 on the top and bottom
	for (int i = -7; i <= 7; i++) {
		pos = LevelHelper::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2 - i * (GAME_WIDTH/2 - horzDistFromEdge)/8.0, GAME_HEIGHT/2, GAME_HEIGHT/2 - mineSpacing);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "mines"));
		pos = LevelHelper::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2 - i * (GAME_WIDTH/2 - horzDistFromEdge)/8.0, GAME_HEIGHT/2, GAME_HEIGHT/2 - mineSpacing);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "mines"));
	}

	//7*2 along the center
	for (int i = 1; i <= 7; i++) {
		//if (i == 0) { continue; }
		pos = LevelHelper::getSymmetricPowerupPositions_UD(0, GAME_WIDTH/2, GAME_HEIGHT/2, i * (GAME_HEIGHT/2 - mineSpacing)/8.0);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "mines"));
		pos = LevelHelper::getSymmetricPowerupPositions_UD(1, GAME_WIDTH/2, GAME_HEIGHT/2, i * (GAME_HEIGHT/2 - mineSpacing)/8.0);
		PowerupManager::pushPowerup(new PowerSquare(pos.x, pos.y, "vanilla-extra", "mines"));
		//JS did just 14 in a row, not 7 pairs
	}

	//it's possible to condense the powerups into the one or two loops, but I don't care
}

Level* MineHeavenLevel::factory() {
	return new MineHeavenLevel();
}

MineHeavenLevel::MineHeavenLevel() {
	PositionHolder* wallArray = LevelHelper::getClassicWallPositions();

	GenericFactoryConstructionData constructionData;
	effects.push_back(LevelManager::makeLevelEffect("vanilla-extra", "ice", constructionData));
	double* temp = new double[4]{ wallArray[0].x+32, 0, wallArray[3].x - (wallArray[0].x+32), GAME_HEIGHT };
	constructionData = GenericFactoryConstructionData(4, temp);
	effects.push_back(LevelManager::makeLevelEffect("vanilla", "minefield", constructionData));

	delete[] wallArray;
}