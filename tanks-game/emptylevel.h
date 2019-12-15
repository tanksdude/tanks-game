#pragma once
#include "level.h"
#include "wall.h"

class EmptyLevel : public Level {
public:
	void initialize();
	void tick() { return; }
	void draw() { return; }
	EmptyLevel();
};