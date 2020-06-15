#include "emptylevel.h"

std::unordered_map<std::string, double> EmptyLevel::getWeights() {
	std::unordered_map<std::string, double> weights;
	weights.insert({ "vanilla", .125 });
	weights.insert({ "random-vanilla", .125 });
	weights.insert({ "random", .125 });
	return weights;
}

void EmptyLevel::initialize() {
	return;
}

Level* EmptyLevel::factory() {
	return new EmptyLevel();
}

EmptyLevel::EmptyLevel() { return; }
