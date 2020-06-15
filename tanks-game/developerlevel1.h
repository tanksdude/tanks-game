#pragma once
#include "level.h"

class DeveloperLevel1 : public Level {
public:
	std::string getName() { return "dev1"; }
	std::vector<std::string> getLevelTypes() {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	void initialize();
	
	DeveloperLevel1();
	static Level* factory();
};
