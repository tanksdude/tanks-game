#pragma once
class Power;

#include <string>
#include <vector>
#include <unordered_map>

#include "color-value-holder.h"
#include "tank-power.h"
#include "bullet-power.h"

class Power {
public:
	virtual std::vector<std::string> getPowerTypes() const = 0;
	virtual std::unordered_map<std::string, float> getWeights() const = 0; //intended range: (0,1]
	virtual std::vector<std::string> getPowerAttributes() const; //stuff like "can stack", "can mix" ("can X" is only a suggestion)

	virtual std::string getName() const = 0;
	//static std::string getClassName(); //tankpowers and bulletpowers use this (so they can have the same color)
	virtual ColorValueHolder getColor() const = 0;
	//static ColorValueHolder getClassColor();
	virtual float getColorImportance() const { return 0; }
	//static float getClassColorImportance();

	virtual TankPower* makeTankPower() const = 0;
	virtual BulletPower* makeBulletPower() const = 0;
	//virtual HazardPower* makeHazardPower() const = 0;

	virtual ~Power() { return; }
	//static Power* factory();
};
