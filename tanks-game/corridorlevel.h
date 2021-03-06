#pragma once
#include "level.h"

class CorridorLevel : public Level {
	//referred to as "evened corridors" in JS Tanks
public:
	std::string getName() { return "corridor"; }
	std::vector<std::string> getLevelTypes() override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() override;
	void initialize();

	CorridorLevel();
	static Level* factory();
};
