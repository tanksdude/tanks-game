#pragma once
#include "../level.h"

class TerrifyingChaosLevel : public Level {
public:
	virtual std::string getName() const override { return "terrifying_chaos"; }
	virtual ColorValueHolder getDefaultColor() const override;
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla", "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	TerrifyingChaosLevel();
	virtual ~TerrifyingChaosLevel() { return; }
	static Level* factory();
};
