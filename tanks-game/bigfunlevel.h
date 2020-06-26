#pragma once
#include "level.h"

class BigFunLevel : public Level {
	//basically the random level but only the early powerups and no hazards
public:
	std::string getName() { return "bigfun"; }
	std::vector<std::string> getLevelTypes() override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() override;
	void initialize();

	BigFunLevel();
	static Level* factory();
};
