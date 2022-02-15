#pragma once
#include "level.h"

class UnnamedLevel3 : public Level {
public:
	virtual std::string getName() const override { return "unnamed3"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0.75f, 0.25f, 0.25f); } //JS: #CC4444
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	UnnamedLevel3();
	virtual ~UnnamedLevel3() { return; }
	static Level* factory();
};
