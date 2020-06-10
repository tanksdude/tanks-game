#pragma once
#include "level.h"

class FewObstaclesLevel : public Level {
	//really needs a better name
public:
	std::string getName() { return "few obstacles"; }
	void initialize();

	FewObstaclesLevel();
	static Level* factory();
};
