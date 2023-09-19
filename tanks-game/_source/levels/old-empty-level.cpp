#include "old-empty-level.h"

#include "../constants.h"

#include "../reset-things.h"
#include "../level-helper.h"

std::unordered_map<std::string, float> OldEmptyLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "old", .25f });
	weights.insert({ "random-old", .25f });
	return weights;
}

void OldEmptyLevel::initialize() {
	ResetThings::tankPositionReset(40, GAME_HEIGHT/2);

	//in JS, power mixing was turned off (yeah, not super important on this level)
	ColorValueHolder color = getDefaultColor();

	LevelHelper::pushClassicWalls(color);
}

Level* OldEmptyLevel::factory() {
	return new OldEmptyLevel();
}

OldEmptyLevel::OldEmptyLevel() { return; }
