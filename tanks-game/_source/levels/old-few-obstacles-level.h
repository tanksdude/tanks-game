#pragma once
#include "../level.h"

class OldFewObstaclesLevel : public Level {
public:
	virtual std::string getName() const override { return "old_few_obstacles"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0x44/255.0, 0xCC/255.0, 0x99/255.0); }
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	OldFewObstaclesLevel();
	virtual ~OldFewObstaclesLevel() { return; }
	static Level* factory();
};
