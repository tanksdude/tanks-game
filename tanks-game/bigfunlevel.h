#pragma once
#include "level.h"

class BigFunLevel : public Level {
	//basically the random level but only the early powerups and no hazards
public:
	virtual std::string getName() const override { return "bigfun"; }
	virtual ColorValueHolder getDefaultColor() const override;
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual void initialize() override;

	BigFunLevel();
	virtual ~BigFunLevel() { return; }
	static Level* factory();
};
