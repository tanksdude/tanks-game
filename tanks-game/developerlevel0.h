#pragma once
#include "level.h"

class DeveloperLevel0 : public Level {
public:
	std::string getName() { return "dev0"; }
	std::vector<std::string> getLevelTypes() {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	void initialize();
	
	DeveloperLevel0();
	static Level* factory();
};
