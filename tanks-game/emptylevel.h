#pragma once
#include "level.h"

class EmptyLevel : public Level {
public:
	std::string getName() const override { return "empty"; }
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;
	
	EmptyLevel();
	static Level* factory();
};
