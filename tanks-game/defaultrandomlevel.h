#pragma once
#include "level.h"

class DefaultRandomLevel : public Level {
protected:
	ColorValueHolder currentColor;
public:
	std::string getName() const override { return "default_random"; }
	ColorValueHolder getDefaultColor() const override;
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;

	DefaultRandomLevel();
	//~DefaultRandomLevel();
	static Level* factory();
};
