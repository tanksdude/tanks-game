#pragma once
#include "level.h"

class SneakyRewardLevel : public Level {
	//the name is actually somewhat okay, right?
	//"annoying reward" is another possibility
public:
	virtual std::string getName() const override { return "sneaky_reward"; }
	virtual ColorValueHolder getDefaultColor() const override;
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	SneakyRewardLevel();
	virtual ~SneakyRewardLevel() { return; }
	static Level* factory();
};
