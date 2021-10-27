#include "leveleffect.h"

std::vector<std::string> LevelEffect::getLevelEffectTypes() const {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
	return types;
}

std::unordered_map<std::string, float> LevelEffect::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "random", .5f });
	return weights;
}

//level effect notes:
/*
multiple level effects can happen on the same level (can they interact with each other?)

list of (eventual) level effects:
invisibility? (no one appreciates it though)
|wind
ice (no one really likes it, as expected)
mines

I need more ideas for level effects...

*/
