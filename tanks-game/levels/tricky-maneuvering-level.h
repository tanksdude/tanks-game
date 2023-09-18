#pragma once
#include "../level.h"

class TrickyManeuveringLevel : public Level {
	//was just referred to as "wind" (as in, "wind" level) in JS Tanks
public:
	virtual std::string getName() const override { return "tricky_maneuvering"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0.5f, 0.75f, 1.0f); } //JS: #88CCFF
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla", "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;
	virtual void tick() override;

	TrickyManeuveringLevel();
	virtual ~TrickyManeuveringLevel() { return; }
	static Level* factory();
};
