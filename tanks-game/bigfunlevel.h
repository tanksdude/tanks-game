#pragma once
#include "level.h"
#include "colorvalueholder.h"

class BigFunLevel : public Level {
	//basically the random level but only the early powerups and no hazards
public:
	std::string getName() { return "bigfun"; }
	void initialize();
	void tick() { return; }
	void draw() { return; }
	BigFunLevel();
};