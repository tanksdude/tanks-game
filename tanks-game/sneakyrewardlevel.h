#pragma once
#include "level.h"

class SneakyRewardLevel : public Level {
	//the name is actually somewhat okay, right?
	//"annoying reward" is another possibility
public:
	std::string getName() const override { return "sneaky_reward"; }
	ColorValueHolder getDefaultColor() const override;
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;

	SneakyRewardLevel();
	static Level* factory();
};
