#pragma once
#include "level.h"

class DeveloperLevel0 : public Level {
protected:
	ColorValueHolder currentColor; //each dev0 level has a different color
public:
	std::string getName() const override { return "dev0"; }
	ColorValueHolder getDefaultColor() const override;
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;

	DeveloperLevel0();
	static Level* factory();
};
