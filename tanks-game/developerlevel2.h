#pragma once
#include "level.h"

class DeveloperLevel2 : public Level {
public:
	std::string getName() { return "dev2"; }
	std::vector<std::string> getLevelTypes() {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	std::unordered_map<std::string, double> getWeights() {
		std::unordered_map<std::string, double> weights;
		weights.insert({ "dev", .5 });
		weights.insert({ "random-dev", .5 });
		return weights;
	}
	void initialize();
	
	DeveloperLevel2();
	static Level* factory();
};
