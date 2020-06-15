#pragma once
#include "level.h"

class ConcealedPowerupsLevel : public Level {
	//definitely needs a better name
public:
	std::string getName() { return "concealed powerups"; }
	std::vector<std::string> getLevelTypes() {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random", "old", "random-old" };
		return types;
	}
	void initialize();

	ConcealedPowerupsLevel();
	static Level* factory();
};
