#pragma once
#include "level.h"

class DeveloperLevel1 : public Level {
public:
	std::string getName() const override { return "dev1"; }
	ColorValueHolder getDefaultColor() const override;
	std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;

	DeveloperLevel1();
	static Level* factory();
};
