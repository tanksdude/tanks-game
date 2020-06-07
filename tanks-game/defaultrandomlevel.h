#pragma once
#include "level.h"

class DefaultRandomLevel : public Level {
public:
	std::string getName() { return "default random"; }
	void initialize();

	DefaultRandomLevel();
	//~DefaultRandomLevel();
	static Level* factory();
};
