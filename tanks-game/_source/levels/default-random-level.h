#pragma once
#include "../level.h"

class DefaultRandomLevel : public Level {
protected:
	ColorValueHolder currentColor;

public:
	virtual std::string getName() const override { return "default_random"; }
	virtual ColorValueHolder getDefaultColor() const override;
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	DefaultRandomLevel();
	virtual ~DefaultRandomLevel() { return; }
	static Level* factory();
};
