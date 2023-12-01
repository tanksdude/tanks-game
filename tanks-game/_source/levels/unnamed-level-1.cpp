#include "unnamed-level-1.h"

#include "../constants.h"

#include "../reset-things.h"
#include "../level-helper.h"
#include "../powerup-manager.h"
#include "../wall-manager.h"
#include "../hazard-manager.h"

ColorValueHolder UnnamedLevel1::getDefaultColor() const {
	return ColorValueHolder(1, 1, 1);
}

std::unordered_map<std::string, float> UnnamedLevel1::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "dev", 0.5f });
	weights.insert({ "random-dev", 0.5f });
	return weights;
}

void UnnamedLevel1::initialize() {
	ResetThings::tankPositionReset();

	ColorValueHolder color = getDefaultColor();
	//PositionHolder pos;
	//GenericFactoryConstructionData constructionData;
	//double* posArr;

	//outer walls
	LevelHelper::pushSymmetricWalls_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, 20, 20+20+40, 160, 20, color);
	LevelHelper::pushSymmetricWalls_LR(GAME_WIDTH/2, GAME_HEIGHT/2, 20+160-20, 20, (20+20+40)*2, color);

	//inner walls
	LevelHelper::pushSymmetricWalls_UD(GAME_WIDTH/2, GAME_HEIGHT/2, 20, (20+160-20-40)*2, 20, color);

	LevelHelper::pushSymmetricPowerups_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, 20+160+40, (GAME_HEIGHT/2 - (20+20+40+20))/2 + (20+20+40+20), "vanilla", "multishot");

	LevelHelper::pushSymmetricPowerups_Corners(GAME_WIDTH/2, GAME_HEIGHT/2, 20+160-20-20, 20+20+20, "vanilla", "speed");

	PowerupManager::pushPowerup(new PowerSquare(GAME_WIDTH/2, GAME_HEIGHT/2, "vanilla-extra", "tracking")); //TOOD: maybe wall sparks instead?

	//this level is basically a combination of hiding places, evened corridors, and winning path...

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
