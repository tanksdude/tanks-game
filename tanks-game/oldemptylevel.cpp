#include "oldemptylevel.h"
#include "constants.h"
#include "randomlevel.h"
#include "tankmanager.h"
#include "wallmanager.h"
#include "resetthings.h"

std::unordered_map<std::string, double> OldEmptyLevel::getWeights() {
	std::unordered_map<std::string, double> weights;
	weights.insert({ "old", .125 });
	weights.insert({ "random-old", .125 });
	return weights;
}

void OldEmptyLevel::initialize() {
	ResetThings::tankPositionReset(TankManager::getTank(0), TankManager::getTank(1), 40, GAME_HEIGHT/2, true);

	ColorValueHolder color = ColorValueHolder(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 4; i++) {
		PositionHolder pos = RandomLevel::getSymmetricWallPositions_Corners(i, GAME_WIDTH/2, GAME_HEIGHT/2, 240-32, GAME_HEIGHT/2-128, 32, 128);
		WallManager::pushWall(new Wall(pos.x, pos.y, 32, 128, color));
	}
}

Level* OldEmptyLevel::factory() {
	return new OldEmptyLevel();
}

OldEmptyLevel::OldEmptyLevel() { return; }
