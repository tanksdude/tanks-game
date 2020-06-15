#include "level.h"

/*
//this shouldn't really need to be changed, so I'll leave it in the header file
std::vector<std::string> Level::getLevelTypes() {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
	return types;
}
*/

std::unordered_map<std::string, double> Level::getWeights() {
	std::unordered_map<std::string, double> weights;
	weights.insert({ "vanilla", .5 });
	weights.insert({ "random-vanilla", .5 });
	weights.insert({ "random", .5 });
	return weights;
}
