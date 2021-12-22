#pragma once
#include "level.h"

class LoneTurretLevel : public Level {
	//called "turret level" in JS Tanks (it was the first level with a turret...)
public:
	virtual std::string getName() const override { return "lone_turret"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0.75f, 0.25f, 0.25f); } //JS: #CC4444
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla", "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	LoneTurretLevel();
	virtual ~LoneTurretLevel() { return; }
	static Level* factory();
};
