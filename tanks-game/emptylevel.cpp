#include "emptylevel.h"

std::unordered_map<std::string, float> EmptyLevel::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", .125f });
	weights.insert({ "random-vanilla", .125f });
	weights.insert({ "random", .125f });
	return weights;
}

void EmptyLevel::initialize() {
	return;
}

Level* EmptyLevel::factory() {
	return new EmptyLevel();
}

EmptyLevel::EmptyLevel() { return; }
