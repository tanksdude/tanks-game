#include "empty-level.h"

#include "../constants.h"

#include "../reset-things.h"

std::unordered_map<std::string, float> EmptyLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 0.25f });
	weights.insert({ "random-vanilla", 0.25f });
	weights.insert({ "old", 0.25f }); //not really necessary
	weights.insert({ "random", 0.125f }); //it's boring so it shouldn't appear very often
	return weights;
}

void EmptyLevel::initialize() {
	ResetThings::tankPositionReset(ResetThings::default_tankToEdgeDist, GAME_HEIGHT/2);
}

Level* EmptyLevel::factory() {
	return new EmptyLevel();
}

EmptyLevel::EmptyLevel() { return; }
