#pragma once
#include "level.h"

class ConcealedPowerupsLevel : public Level {
	//definitely needs a better name
public:
	virtual std::string getName() const override { return "concealed_powerups"; }
	virtual ColorValueHolder getDefaultColor() const override { return ColorValueHolder(0xAA/255.0, 0xBB/255.0, 0x99/255.0); }
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual void initialize() override;

	ConcealedPowerupsLevel();
	virtual ~ConcealedPowerupsLevel() { return; }
	static Level* factory();
};
