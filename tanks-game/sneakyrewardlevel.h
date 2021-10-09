#pragma once
#include "level.h"

class SneakyRewardLevel : public Level {
	//the name is actually somewhat okay, right?
	//"annoying reward" is another possibility
public:
	std::string getName() const override { return "sneaky_reward"; }
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "vanilla-extra", .5f });
		weights.insert({ "random-vanilla", .125f }); //should this even appear?
		return weights;
	}
	void initialize() override;

	SneakyRewardLevel();
	static Level* factory();
};
