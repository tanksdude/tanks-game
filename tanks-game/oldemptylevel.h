#pragma once
#include "level.h"

class OldEmptyLevel : public Level {
public:
	std::string getName() { return "oldempty"; }
	std::vector<std::string> getLevelTypes() {
		std::vector<std::string> types = std::vector<std::string>{ "old", "random-old" };
		return types;
	}
	std::unordered_map<std::string, float> getWeights();
	void initialize();
	
	OldEmptyLevel();
	static Level* factory();
};