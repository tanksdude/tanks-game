#include "untraditional-divider-level.h"

#include "../constants.h"
#include <iostream>
#include "../rng.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"
#include "../level-manager.h"

#include "../level-effects/respawning-powerups-level-effect.h"
#include "../level-effects/magnetism-level-effect.h"

ColorValueHolder UntraditionalDividerLevel::getDefaultColor() const {
	return ColorValueHolder(0x76/255.0, 0xC6/255.0, 0x7D/255.0); //darker and less red shield color
}

std::unordered_map<std::string, float> UntraditionalDividerLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 0.5f });
	weights.insert({ "random-vanilla", 0.5f });
	return weights;
}

void UntraditionalDividerLevel::initialize() {
	ResetThings::tankPositionReset(ResetThings::default_tankToEdgeDist, GAME_HEIGHT*(3.0/5), 2);

	ColorValueHolder color = getDefaultColor();
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	std::string* types;
	std::string* names;
	//PowerSquare* p;

	LevelEffect* le = nullptr;
	for (int i = 0; i < effects.size(); i++) {
		if (effects[i]->getName() == "respawning_powerups") {
			le = effects[i];
			break;
		}
	}
	if (le == nullptr) {
		throw std::logic_error("ERROR: \"" + getName() + "\" level does not have \"respawning_powerups\" level effect!");
	}
	RespawningPowerupsLevelEffect* respawning = static_cast<RespawningPowerupsLevelEffect*>(le);

	le = nullptr;
	for (int i = 0; i < effects.size(); i++) {
		if (effects[i]->getName() == "magnetism") {
			le = effects[i];
			break;
		}
	}
	if (le == nullptr) {
		throw std::logic_error("ERROR: \"" + getName() + "\" level does not have \"magnetism\" level effect!");
	}
	MagnetismLevelEffect* magnetism = static_cast<MagnetismLevelEffect*>(le);
	//TODO: should this be the preferred way of getting specific level effects?

	WallManager::pushWall(new Wall(GAME_WIDTH/2 - 240, GAME_HEIGHT/2 - 10, 240*2, 10*2, color));

	for (int i = 0; i < 4; i++) {
		PositionHolder posOuter = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-20,           10+50, 20, GAME_HEIGHT/2 - (10+50));
		posArr = new double[4]{ posOuter.x, posOuter.y, 20, GAME_HEIGHT/2 - (10+50) };
		constructionData = GenericFactoryConstructionData(4, posArr);
		HazardManager::pushRectHazard("vanilla", "lava", constructionData);

		PositionHolder posInner = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-20 - (80+20), 10+50, 20, GAME_HEIGHT/2 - (10+50));
		posArr = new double[4]{ posInner.x, posInner.y, 20, GAME_HEIGHT/2 - (10+50) };
		constructionData = GenericFactoryConstructionData(4, posArr);
		HazardManager::pushRectHazard("vanilla", "lava", constructionData);
	}

	types = new std::string[2]{ "vanilla", "vanilla" };
	names = new std::string[2]{ "invincible", "speed" }; //TODO: this probably needs two speed
	LevelHelper::pushSymmetricPowerups_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, 240-20 - 80/2, GAME_HEIGHT/2 - 20, types, names, 2);
	delete[] types; delete[] names;
	respawning->watchLastPowerSquaresPushed(4, 1000);
	magnetism->watchLastPowerSquaresPushed(4);

	const int turretCount = 3;
	for (int j = 0; j < turretCount; j++) {
		for (int i = 0; i < 4; i++) {
			pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-20 - (80+20) + 20/2, (10+50) + j*(GAME_HEIGHT/2 - (10+50) - 20)/(turretCount-1) + 20/2);
			posArr = new double[3]{ pos.x, pos.y, PI * (i%2) };
			constructionData = GenericFactoryConstructionData(3, posArr);
			HazardManager::pushCircleHazard("vanilla", "stationary_turret", constructionData);
		}
	}

	//LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2 + (10+50) + (GAME_HEIGHT/2 - (10+50))/2, 40, 40,
	//	"vanilla", "multishot", "vanilla", "fire");
	//LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2 - (10+50) - (GAME_HEIGHT/2 - (10+50))/2, 40, 40,
	//	"vanilla", "multishot", "vanilla", "fire");
	LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, 80, (10+50) + (GAME_HEIGHT/2 - (10+50))/2 + 40,
		"vanilla-extra", "barrier", "vanilla", "triple");
	LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, 80, (10+50) + (GAME_HEIGHT/2 - (10+50))/2 - 40,
		"vanilla", "multishot", "vanilla", "fire");

	types = new std::string[2]{ "vanilla", "vanilla" };
	names = new std::string[2]{ "homing", "wallhack" };
	LevelHelper::pushSymmetricPowerups_UD(GAME_WIDTH/2, GAME_HEIGHT/2, (10+50) + (GAME_HEIGHT/2 - (10+50))/2, types, names, 2);
	delete[] types; delete[] names;
	respawning->watchLastPowerSquaresPushed(2);

	posArr = new double[3]{ GAME_WIDTH/2, GAME_HEIGHT/2, 24 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard("vanilla", "no_bullet_zone", constructionData); //excluding random levels, this is currently the only circular no bullet zone in the game...

	//screw it, put down some circular lightnings (because these will currently be the only ones in the game)
	posArr = new double[3]{ GAME_WIDTH/2, 0, 16 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard("vanilla", "lightning", constructionData);
	posArr = new double[3]{ GAME_WIDTH/2, GAME_HEIGHT, 16 };
	constructionData = GenericFactoryConstructionData(3, posArr);
	HazardManager::pushCircleHazard("vanilla", "lightning", constructionData);

	//alternate screw it:
	/*
	for (int i = 0; i < 4; i++) {
		pos = LevelHelper::getSymmetricPowerupPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-20 - 80/2, GAME_HEIGHT/2 - 20);
		posArr = new double[3]{ pos.x, pos.y, 16 };
		constructionData = GenericFactoryConstructionData(3, posArr);
		HazardManager::pushCircleHazard("vanilla", "lightning", constructionData);
	}
	*/
}

Level* UntraditionalDividerLevel::factory() {
	return new UntraditionalDividerLevel();
}

UntraditionalDividerLevel::UntraditionalDividerLevel() {
	bool* temp1 = new bool[1]{ false };
	GenericFactoryConstructionData constructionData(1, temp1);
	effects.push_back(LevelManager::makeLevelEffect("vanilla", "respawning_powerups", constructionData));
	bool* temp2 = new bool[1]{ false };
	constructionData = GenericFactoryConstructionData(1, temp2);
	effects.push_back(LevelManager::makeLevelEffect("vanilla", "magnetism", constructionData));

	std::shared_ptr<int[]> mineCount = std::shared_ptr<int[]>(new int[2]{ 4, 16 }, GenericFactoryConstructionData::deleteArrFunc);
	for (int i = 0; i < 4; i++) {
		PositionHolder posOuter = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-20,           10+50, 20, GAME_HEIGHT/2 - (10+50));
		PositionHolder posInner = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-20 - (80+20), 10+50, 20, GAME_HEIGHT/2 - (10+50));

		double* minefieldData = new double[4]{ (posOuter.x < posInner.x ? posOuter.x : posInner.x) + 20, posOuter.y, 80, GAME_HEIGHT/2 - (10+50) };
		GenericFactoryConstructionData constructionData(4, minefieldData);
		constructionData.pushData(2, mineCount);
		effects.push_back(LevelManager::makeLevelEffect("vanilla", "minefield", constructionData));
	}
}
