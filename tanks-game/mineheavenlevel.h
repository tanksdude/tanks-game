#pragma once
#include "level.h"

class MineHeavenLevel : public Level {
	//was just referred to as "ice and mines" in JS Tanks
public:
	virtual std::string getName() const override { return "mine_heaven"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0.25f, 0.25f, 0.25f); } //JS: #444444
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla", "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual void initialize() override;
	virtual void tick() override;

	MineHeavenLevel();
	virtual ~MineHeavenLevel() { return; }
	static Level* factory();
};
