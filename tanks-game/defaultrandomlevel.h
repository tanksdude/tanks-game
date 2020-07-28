#pragma once
#include "level.h"

class DefaultRandomLevel : public Level {
public:
	std::string getName() const override { return "default random"; }
	std::unordered_map<std::string, float> getWeights() const override;
	void initialize() override;

	DefaultRandomLevel();
	//~DefaultRandomLevel();
	static Level* factory();
};
