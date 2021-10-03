#pragma once
#include "level.h"

class ConcealedPowerupsLevel : public Level {
	//definitely needs a better name
public:
	std::string getName() const override { return "concealed_powerups"; }
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;

	ConcealedPowerupsLevel();
	static Level* factory();
};
