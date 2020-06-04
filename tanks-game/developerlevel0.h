#pragma once
#include "level.h"

class DeveloperLevel0 : public Level {
public:
	std::string getName() { return "dev0"; }
	void initialize();
	void tick() { return; }
	void draw() { return; }
	DeveloperLevel0();
};
