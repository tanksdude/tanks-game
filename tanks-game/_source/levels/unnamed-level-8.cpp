#include "unnamed-level-8.h"

#include "../constants.h"
#include <iostream>
#include "../rng.h"

//#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"
#include "../level-manager.h"
#include "../tank-manager.h"

ColorValueHolder UnnamedLevel8::getDefaultColor() const {
	//TODO: very dark green?
	return ColorValueHolder(1, 1, 1);
}

std::unordered_map<std::string, float> UnnamedLevel8::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 1.0f });
	weights.insert({ "random-dev", 1.0f });
	return weights;
}

void UnnamedLevel8::initialize() {
	TankManager::getTank(0)->x = GAME_WIDTH/2 - 240/4;
	TankManager::getTank(1)->x = GAME_WIDTH/2 + 240/4;
	TankManager::getTank(0)->y = GAME_HEIGHT - 20;
	TankManager::getTank(1)->y = 20;
	TankManager::getTank(0)->velocity.setAngle(-PI/2);
	TankManager::getTank(1)->velocity.setAngle(PI/2);

	ColorValueHolder color = getDefaultColor();
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	std::string* types;
	std::string* names;

	LevelHelper::pushSymmetricWalls_UD(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_HEIGHT/2 - (40+20), 240, 20, color);

	posArr = new double[3]{ GAME_WIDTH/2, GAME_HEIGHT/2, PI/2 * (floor(RNG::randFunc()*2)*2-1) };
	int* childArr = new int[2]{ 32, 16 }; //this is a lot
	constructionData = GenericFactoryConstructionData(3, posArr);
	constructionData.pushData(2, childArr);
	HazardManager::pushCircleHazard("vanilla", "ginormous_turret", constructionData);

	LevelHelper::pushSymmetricPowerups_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - 20, GAME_HEIGHT/2 - 20, "vanilla-extra", "barrier");

	types = new std::string[2]{ "vanilla", "vanilla" };
	names = new std::string[2]{ "speed", "wallhack" }; //what an ugly color
	LevelHelper::pushSymmetricPowerups_LR(GAME_WIDTH/2, GAME_HEIGHT/2, 128, types, names, 2);
	delete[] types; delete[] names;

	LevelHelper::pushSymmetricPowerups_LR(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - 20, "vanilla", "big");
	LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - 20, 40,
		"vanilla", "multishot", "dev", "triple_spread");

	LevelHelper::pushSymmetricPowerups_Corners_Alternate(GAME_WIDTH/2, GAME_HEIGHT/2, 240/2 - 20/2, GAME_HEIGHT/2 - 40 - 20/2,
		"dev", "edge_sparks", "vanilla", "blast"); //maybe homing instead of edge sparks, though homing is very powerful on this level
}

Level* UnnamedLevel8::factory() {
	return new UnnamedLevel8();
}

UnnamedLevel8::UnnamedLevel8() { 
	GenericFactoryConstructionData constructionData;
	effects.push_back(LevelManager::makeLevelEffect("vanilla", "respawning_powerups", constructionData));
}
