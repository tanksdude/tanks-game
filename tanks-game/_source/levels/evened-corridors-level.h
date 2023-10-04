#pragma once
#include "../level.h"

class EvenedCorridorsLevel : public Level {
public:
	virtual std::string getName() const override { return "evened_corridors"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0.125f, 0.5f, 1.0f); } //JS: #2288FF
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	EvenedCorridorsLevel();
	virtual ~EvenedCorridorsLevel() { return; }
	static Level* factory();
};
