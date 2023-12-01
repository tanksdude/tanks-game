#pragma once
#include "../level.h"

class ShowcaseLevel1 : public Level {
public:
	virtual std::string getName() const override { return "showcase1"; }
	virtual ColorValueHolder getDefaultColor() const override;
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	ShowcaseLevel1();
	virtual ~ShowcaseLevel1() { return; }
	static Level* factory();
};
