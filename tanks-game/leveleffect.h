#pragma once
class LevelEffect;

#include "drawablething.h"
#include "level.h"
#include <vector>
#include <string>
#include <unordered_map>

//TODO: level effects basically need to be powers but for levels (needs to be able to override some things in GameMainLoop, I think)
class LevelEffect {
public:
	virtual std::string getName() const = 0;
	virtual std::vector<std::string> getLevelEffectTypes() const;
	virtual std::unordered_map<std::string, float> getWeights() const; //intended range: (0,1]

	virtual void apply() = 0;
	virtual void tick(const Level* parent) = 0; //update variables and stuff
	virtual void doEffects(Level* parent) = 0; //do what needs to be done

	//should LevelEffect extend DrawableThing?
	virtual void draw() const { return; }
	//virtual void draw(double xpos, double ypos) const = 0; //would this be used?
	//virtual void poseDraw() const = 0;

	virtual ~LevelEffect() { return; }
	static LevelEffect* factory();
};
