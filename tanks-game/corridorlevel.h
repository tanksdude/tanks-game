#pragma once
#include "level.h"
#include "wall.h"
#include "colorvalueholder.h"

class CorridorLevel : public Level {
	//called "evened corridors" in JS Tanks
public:
	void initialize();
	void tick() { return; }
	void draw() { return; }
	CorridorLevel();
};