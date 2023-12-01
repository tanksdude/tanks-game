#pragma once
#include "../level.h"

class OldEvenedCorridorsLevel : public Level {
public:
	virtual std::string getName() const override { return "old_evened_corridors"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0x22/255.0, 0x88/255.0, 1.0f); }
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	OldEvenedCorridorsLevel();
	virtual ~OldEvenedCorridorsLevel() { return; }
	static Level* factory();
};
