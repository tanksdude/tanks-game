#pragma once
#include "level.h"

class ManyHazardsLevel : public Level {
	//called "hazards everywhere" in JS Tanks
	//needs a better name
public:
	std::string getName() const override { return "many hazards"; }
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;

	ManyHazardsLevel();
	static Level* factory();
};
