#include "oldemptylevel.h"
#include "constants.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "wallmanager.h"
#include "resetthings.h"

std::unordered_map<std::string, float> OldEmptyLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "old", .125f });
	weights.insert({ "random-old", .125f });
	return weights;
}

void OldEmptyLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40, GAME_HEIGHT/2);

	//in JS, power mixing was turned off (yeah, not super important on this level)
	ColorValueHolder color = getDefaultColor();
	//int tempRand;
	PositionHolder pos;
	//std::string* paras;

	for (int i = 0; i < 4; i++) {
		//classic JS walls
		pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, GAME_WIDTH/2-40*2-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}
}

Level* OldEmptyLevel::factory() {
	return new OldEmptyLevel();
}

OldEmptyLevel::OldEmptyLevel() { return; }
