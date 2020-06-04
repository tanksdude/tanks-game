#pragma once
#include "level.h"

class CorridorLevel : public Level {
	//referred to as "evened corridors" in JS Tanks
public:
	std::string getName() { return "corridor"; }
	void initialize();
	void tick() { return; }
	void draw() { return; }
	CorridorLevel();
};
