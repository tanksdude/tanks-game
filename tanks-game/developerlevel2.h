#pragma once
#include "level.h"

class DeveloperLevel2 : public Level {
public:
	std::string getName() const override { return "dev2"; }
	ColorValueHolder getDefaultColor() const override;
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;

	DeveloperLevel2();
	static Level* factory();
};
