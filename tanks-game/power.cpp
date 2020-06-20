#include "power.h"

std::vector<std::string> Power::getPowerTypes() {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla", "supermix-vanilla", "supermix", "random-vanilla", "random" };
	return types;
}

std::unordered_map<std::string, float> Power::getWeights() {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .5f });
	weights.insert({ "supermix-vanilla", .5f });
	weights.insert({ "supermix", .5f });
	weights.insert({ "random-vanilla", .5f });
	weights.insert({ "random", .5f });
	return weights;
}

std::vector<std::string> Power::getPowerAttributes() {
	std::vector<std::string> attributes = std::vector<std::string>{ "stack", "mix" };
	return attributes;
}
