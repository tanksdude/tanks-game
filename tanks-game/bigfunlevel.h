#pragma once
#include "level.h"

class BigFunLevel : public Level {
	//basically the random level but only the early powerups and no hazards
public:
	std::string getName() { return "bigfun"; }
	void initialize();

	BigFunLevel();
	static Level* factory();
};
