#pragma once
#include "leveleffect.h"
#include <vector>
#include <string>
#include <unordered_map>

class Level {
	friend class LevelManager; //actually needs this unlike the other managers because the manager has more control
protected:
	std::vector<LevelEffect*> effects;
	int getNumEffects() { return effects.size(); } //kinda pointless but "clean code"
public:
	virtual std::string getName() = 0;
	virtual std::vector<std::string> getLevelTypes();
	virtual std::unordered_map<std::string, float> getWeights(); //intended range: (0,1]
	virtual void initialize() = 0;
	virtual void tick() { return; }
	virtual void draw() { return; }

	virtual ~Level() { return; }
	static Level* factory();
};
