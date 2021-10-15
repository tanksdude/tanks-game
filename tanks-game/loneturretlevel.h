#pragma once
#include "level.h"

class LoneTurretLevel : public Level {
	//called "turret level" in JS Tanks (it was the first level with a turret...)
public:
	std::string getName() const override { return "lone_turret"; }
	ColorValueHolder getDefaultColor() const override { return ColorValueHolder(.75f, .25f, .25f); } //JS: #CC4444
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla", "old", "random-old" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;

	LoneTurretLevel();
	static Level* factory();
};
