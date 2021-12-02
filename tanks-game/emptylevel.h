#pragma once
#include "level.h"

class EmptyLevel : public Level {
public:
	virtual std::string getName() const override { return "empty"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0.0f, 0.0f, 0.0f); }
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual void initialize() override;

	EmptyLevel();
	virtual ~EmptyLevel() { return; }
	static Level* factory();
};
