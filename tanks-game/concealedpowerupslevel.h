#pragma once
#include "level.h"

class ConcealedPowerupsLevel : public Level {
	//definitely needs a better name
public:
	std::string getName() { return "concealed powerups"; }
	void initialize();

	ConcealedPowerupsLevel();
	static Level* factory();
};
