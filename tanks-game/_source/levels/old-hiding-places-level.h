#pragma once
#include "../level.h"

class OldHidingPlacesLevel : public Level {
public:
	virtual std::string getName() const override { return "old_hiding_places"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0x66/255.0, 0xAA/255.0, 0x22/255.0); }
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	OldHidingPlacesLevel();
	virtual ~OldHidingPlacesLevel() { return; }
	static Level* factory();
};
