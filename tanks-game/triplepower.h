#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class TriplePower : public Power{
protected:
	//static ColorValueHolder classColor;
public:
	static const double angleDiff;

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::string getName() { return TriplePower::getClassName(); }
	static std::string getClassName() { return "triple"; }
	virtual ColorValueHolder getColor() { return TriplePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xCC/255.0, 0xCC/255.0, 0.5f); } //beige

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	TriplePower();
	~TriplePower();
	static Power* factory();
};