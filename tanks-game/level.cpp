#include "level.h"

/*
//this shouldn't really need to be changed, so I'll leave it in the header file
std::vector<std::string> Level::getLevelTypes() {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
	return types;
}
*/

std::unordered_map<std::string, float> Level::getWeights() {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "random", .5f });
	return weights;
}
