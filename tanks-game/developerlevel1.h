#pragma once
#include "level.h"

class DeveloperLevel1 : public Level {
public:
	std::string getName() { return "dev1"; }
	std::vector<std::string> getLevelTypes() override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "dev", .5f });
		weights.insert({ "random-dev", .5f });
		return weights;
	}
	void initialize();
	
	DeveloperLevel1();
	static Level* factory();
};
