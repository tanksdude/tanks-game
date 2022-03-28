#pragma once
#include "level.h"

class TightPatrollingCorridorLevel : public Level {
public:
	virtual std::string getName() const override { return "tight_patrolling_corridor"; }
	virtual ColorValueHolder getDefaultColor() const override;
	virtual std::vector<std::string> getLevelTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random-vanilla" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void initialize() override;

	TightPatrollingCorridorLevel();
	virtual ~TightPatrollingCorridorLevel() { return; }
	static Level* factory();
};
