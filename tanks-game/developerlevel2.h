#pragma once
#include "level.h"

class DeveloperLevel2 : public Level {
public:
	std::string getName() { return "dev2"; }
	void initialize();
	
	DeveloperLevel2();
	static Level* factory();
};
