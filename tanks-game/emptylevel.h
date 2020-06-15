#pragma once
#include "level.h"

class EmptyLevel : public Level {
public:
	std::string getName() { return "empty"; }
	std::unordered_map<std::string, double> getWeights();
	void initialize();
	
	EmptyLevel();
	static Level* factory();
};
