#pragma once
#include "level.h"

#include "constants.h" //Game_ID

class TimedRewardLevel : public Level {
protected:
	Game_ID* movingWalls;

public:
	virtual std::string getName() const override { return "timed_reward"; }
	virtual ColorValueHolder getDefaultColor() const override;
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla", "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;
	virtual void tick() override;

	TimedRewardLevel();
	virtual ~TimedRewardLevel();
	static Level* factory();
};
