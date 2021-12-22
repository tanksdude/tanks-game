#pragma once
#include "level.h"

class ManyHazardsLevel : public Level {
	//called "hazards everywhere" in JS Tanks
public:
	virtual std::string getName() const override { return "many_hazards"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0xDD/255.0, 0.5f, 0.25f); } //JS: #DD8844
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	ManyHazardsLevel();
	virtual ~ManyHazardsLevel() { return; }
	static Level* factory();
};
