#pragma once
#include "../level.h"

class HidingPlacesLevel : public Level {
	//I'm awful at naming things
public:
	virtual std::string getName() const override { return "hiding_places"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0.375f, 0.75f, 0.125f); } //JS: #66AA22
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla", "old", "random-old" };
		//note: keep this in random-vanilla because although it's not a great level, it's always fun to have a level with something "unique" (the mines powerups, since those don't get randomly placed)
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	HidingPlacesLevel();
	virtual ~HidingPlacesLevel() { return; }
	static Level* factory();
};
