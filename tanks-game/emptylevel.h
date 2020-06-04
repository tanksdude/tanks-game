#pragma once
#include "level.h"

class EmptyLevel : public Level {
public:
	std::string getName() { return "empty"; }
	void initialize();
	void tick() { return; }
	void draw() { return; }
	EmptyLevel();
};
