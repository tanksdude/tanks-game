#pragma once
#include "level.h"

class DefaultRandomLevel : public Level {
public:
	std::string getName() { return "default random"; }
	std::unordered_map<std::string, float> getWeights();
	void initialize();

	DefaultRandomLevel();
	//~DefaultRandomLevel();
	static Level* factory();
};
