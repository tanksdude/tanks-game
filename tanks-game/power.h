#pragma once
class Power;

#include <string>
#include <vector>
#include <unordered_map>
#include "colorvalueholder.h"
#include "tankpower.h"
#include "bulletpower.h"

class Power {
public:
	virtual std::vector<std::string> getPowerTypes(); //defined in source file in case I change some values
	virtual std::unordered_map<std::string, float> getWeights(); //intended range: (0,1]
	virtual std::vector<std::string> getPowerAttributes(); //stuff like "can stack", "can mix" ("can X" is only a suggestion)

	virtual std::string getName() = 0;
	//static std::string getClassName(); //tankpowers and bulletpowers use this (so they can have the same color), so make sure to define it!
	virtual ColorValueHolder getColor() = 0;
	//static ColorValueHolder getClassColor();
	virtual double getColorImportance() { return 0; }
	//static double getClassColorImportance();

	virtual TankPower* makeTankPower() = 0;
	virtual BulletPower* makeBulletPower() = 0;
	//virtual HazardPower* makeHazardPower() = 0;

	virtual ~Power() { return; }
	static Power* factory();
};
