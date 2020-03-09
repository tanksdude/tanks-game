#pragma once
#include "level.h"

class DefaultRandomLevel : public Level {
public:
	std::string getName() { return "default random"; }
	void initialize();
	void tick() { return; }
	void draw() { return; }
	DefaultRandomLevel();
};