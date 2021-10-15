#pragma once
#include "level.h"

class LightningCornersLevel : public Level {
	//cut from JS Tanks 'cuz it didn't have anything going for it (I think it was originally made to play around with powerup mixing)
public:
	std::string getName() const override { return "lightning_corners"; }
	ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0x77/255.0, 0x55/255.0f, 0.75f); } //JS: #7755CC
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla", "old" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;

	LightningCornersLevel();
	static Level* factory();
};
