#pragma once
#include "level.h"

class DeveloperLevel1 : public Level {
public:
	std::string getName() { return "dev1"; }
	void initialize();
	void tick() { return; }
	void draw() { return; }
	DeveloperLevel1();
};
