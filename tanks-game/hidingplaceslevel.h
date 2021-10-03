#pragma once
#include "level.h"

class HidingPlacesLevel : public Level {
	//I'm awful at naming things
public:
	std::string getName() const override { return "hiding_places"; }
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "vanilla-extra", .5f });
		weights.insert({ "random-vanilla", .25f });
		return weights;
	}
	void initialize() override;

	HidingPlacesLevel();
	static Level* factory();
};
