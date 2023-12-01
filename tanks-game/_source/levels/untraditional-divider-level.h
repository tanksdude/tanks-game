#pragma once
#include "../level.h"

class UntraditionalDividerLevel : public Level {
public:
	virtual std::string getName() const override { return "untraditional_divider"; }
	virtual ColorValueHolder getDefaultColor() const override;
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	UntraditionalDividerLevel();
	virtual ~UntraditionalDividerLevel() { return; }
	static Level* factory();
};
