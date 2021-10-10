#pragma once
#include "level.h"

class EmptyLevel : public Level {
public:
	std::string getName() const override { return "empty"; }
	ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0.0f, 0.0f, 0.0f); }
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;

	EmptyLevel();
	static Level* factory();
};
