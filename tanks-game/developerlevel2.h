#pragma once
#include "level.h"

class DeveloperLevel2 : public Level {
public:
	std::string getName() { return "dev2"; }
	std::vector<std::string> getLevelTypes() {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	void initialize();
	
	DeveloperLevel2();
	static Level* factory();
};
