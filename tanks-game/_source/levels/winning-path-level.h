#pragma once
#include "../level.h"

class WinningPathLevel : public Level {
	//was just referred to as "invisible" level in JS Tanks
public:
	virtual std::string getName() const override { return "winning_path"; }
	virtual ColorValueHolder getDefaultColor() const override;
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla", "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;
	virtual void tick() override;

	WinningPathLevel();
	virtual ~WinningPathLevel() { return; }
	static Level* factory();
};
