#pragma once
#include "../level.h"

class UnnamedLevel13 : public Level {
public:
	virtual std::string getName() const override { return "unnamed13"; }
	virtual ColorValueHolder getDefaultColor() const override;
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	UnnamedLevel13();
	virtual ~UnnamedLevel13() { return; }
	static Level* factory();
};
