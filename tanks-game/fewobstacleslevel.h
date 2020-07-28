#pragma once
#include "level.h"

class FewObstaclesLevel : public Level {
	//really needs a better name
public:
	std::string getName() const override { return "few obstacles"; }
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		//it's not really an old level (missing the original walls) but whatever
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;

	FewObstaclesLevel();
	static Level* factory();
};
