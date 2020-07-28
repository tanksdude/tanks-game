#pragma once
#include "level.h"

class OldEmptyLevel : public Level {
public:
	std::string getName() const override { return "oldempty"; }
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "old", "random-old" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;
	
	OldEmptyLevel();
	static Level* factory();
};
