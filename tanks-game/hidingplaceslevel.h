#pragma once
#include "level.h"

class HidingPlacesLevel : public Level {
	//I'm awful at naming things
public:
	std::string getName() const override { return "hiding_places"; }
	ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0.375f, 0.75f, 0.125f); }
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;

	HidingPlacesLevel();
	static Level* factory();
};
