#pragma once
#include "level.h"

class OldEmptyLevel : public Level {
public:
	virtual std::string getName() const override { return "old_empty"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0.0f, 0.0f, 0.0f); }
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	OldEmptyLevel();
	virtual ~OldEmptyLevel() { return; }
	static Level* factory();
};
