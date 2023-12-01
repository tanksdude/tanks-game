#pragma once
#include "../level.h"

class FewObstaclesLevel : public Level {
	//really needs a better name
public:
	virtual std::string getName() const override { return "few_obstacles"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0.25f, 0.75f, 0x99/255.0); } //JS: #44CC99
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	FewObstaclesLevel();
	virtual ~FewObstaclesLevel() { return; }
	static Level* factory();
};
