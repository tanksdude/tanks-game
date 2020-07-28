#pragma once
#include "level.h"

class DeveloperLevel0 : public Level {
public:
	std::string getName() const override { return "dev0"; }
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override {
		//TODO: move to source file? not like it really matters
		std::unordered_map<std::string, float> weights;
		weights.insert({ "dev", .5f });
		weights.insert({ "random-dev", .5f });
		return weights;
	}
	void initialize() override;
	
	DeveloperLevel0();
	static Level* factory();
};
