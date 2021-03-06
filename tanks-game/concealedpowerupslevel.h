#pragma once
#include "level.h"

class ConcealedPowerupsLevel : public Level {
	//definitely needs a better name
public:
	std::string getName() { return "concealed powerups"; }
	std::vector<std::string> getLevelTypes() override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() override;
	void initialize();

	ConcealedPowerupsLevel();
	static Level* factory();
};
