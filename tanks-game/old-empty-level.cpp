#include "old-empty-level.h"
#include "constants.h"
#include "level-helper.h"
#include "tank-manager.h"
#include "wall-manager.h"
#include "reset-things.h"

std::unordered_map<std::string, float> OldEmptyLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "old", .25f });
	weights.insert({ "random-old", .25f });
	return weights;
}

void OldEmptyLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40, GAME_HEIGHT/2);

	//in JS, power mixing was turned off (yeah, not super important on this level)
	ColorValueHolder color = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	//GenericFactoryConstructionData constructionData;
	//double* posArr;

	for (int i = 0; i < 4; i++) {
		//classic JS walls
		pos = LevelHelper::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}
}

Level* OldEmptyLevel::factory() {
	return new OldEmptyLevel();
}

OldEmptyLevel::OldEmptyLevel() { return; }
