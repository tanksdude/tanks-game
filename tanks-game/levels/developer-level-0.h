#pragma once
#include "../level.h"

class DeveloperLevel0 : public Level {
protected:
	ColorValueHolder currentColor; //each dev0 level has a different color

public:
	virtual std::string getName() const override { return "dev0"; }
	virtual ColorValueHolder getDefaultColor() const override;
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	DeveloperLevel0();
	virtual ~DeveloperLevel0() { return; }
	static Level* factory();
};
