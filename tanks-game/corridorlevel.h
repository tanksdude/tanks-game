#pragma once
#include "level.h"

class CorridorLevel : public Level {
	//referred to as "evened corridors" in JS Tanks
public:
	std::string getName() { return "corridor"; }
	std::vector<std::string> getLevelTypes() {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random", "old", "random-old" };
		return types;
	}
	void initialize();

	CorridorLevel();
	static Level* factory();
};
