#pragma once
#include "level.h"

class DeveloperLevel0 : public Level {
public:
	std::string getName() { return "dev0"; }
	void initialize();
	
	DeveloperLevel0();
	static Level* factory();
};
