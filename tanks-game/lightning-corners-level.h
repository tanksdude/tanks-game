#pragma once
#include "level.h"

class LightningCornersLevel : public Level {
	//cut from JS Tanks 'cuz it didn't have anything going for it (I think it was originally made to play around with powerup mixing)
public:
	virtual std::string getName() const override { return "lightning_corners"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0x77/255.0, 0x55/255.0, 0.75f); } //JS: #7755CC
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "old" }; //pretty boring so it's not in random-vanilla
		//no random-old since it was cut
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	LightningCornersLevel();
	virtual ~LightningCornersLevel() { return; }
	static Level* factory();
};
