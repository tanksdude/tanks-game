#pragma once
#include "level.h"

class CorridorLevel : public Level {
	//referred to as "evened corridors" in JS Tanks
public:
	std::string getName() const override { return "corridor"; }
	ColorValueHolder getDefaultColor() const override { return ColorValueHolder(.125f, 0.5f, 1.0f); } //JS: #2288FF
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;

	CorridorLevel();
	static Level* factory();
};
