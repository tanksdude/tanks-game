#pragma once
#include "level.h"
#include "windleveleffect.h"

class TrickyManeuveringLevel : public Level {
	//was just referred to as "wind level" in JS
public:
	std::string getName() const override { return "tricky_maneuvering"; }
	ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0.5f, 0.75f, 1.0f); } //JS: #88CCFF
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla", "old" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;
	void tick() override;

	TrickyManeuveringLevel();
	static Level* factory();
};
