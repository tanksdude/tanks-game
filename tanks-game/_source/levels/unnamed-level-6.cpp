#include "unnamed-level-6.h"

#include "../constants.h"
#include <iostream>
#include "../rng.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"
//#include "../level-manager.h"
#include "../tank-manager.h"

ColorValueHolder UnnamedLevel6::getDefaultColor() const {
	return ColorValueHolder(1, 1, 1);
}

std::unordered_map<std::string, float> UnnamedLevel6::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.0f });
	weights.insert({ "random-dev", 0.0f });
	return weights;
}

void UnnamedLevel6::initialize() {
	ResetThings::tankPositionReset(ResetThings::default_tankToEdgeDist, GAME_HEIGHT - 2*20, 2);
	if (TankManager::getTank(0)->getY() > GAME_HEIGHT/2) {
		TankManager::getTank(0)->velocity.changeAngle(-PI/2);
		TankManager::getTank(1)->velocity.changeAngle(-PI/2);
	} else {
		TankManager::getTank(0)->velocity.changeAngle(PI/2);
		TankManager::getTank(1)->velocity.changeAngle(PI/2);
	}

	ColorValueHolder color = getDefaultColor();
	PositionHolder pos;
	GenericFactoryConstructionData constructionData;
	double* posArr;
	std::string* types;
	std::string* names;

	//LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - 80, GAME_HEIGHT/2 - (40+20), 80, 20, color);
	LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2 - (40+20), GAME_HEIGHT/2 - 60, 20, 60, color);


}

Level* UnnamedLevel6::factory() {
	return new UnnamedLevel6();
}

UnnamedLevel6::UnnamedLevel6() { return; }
