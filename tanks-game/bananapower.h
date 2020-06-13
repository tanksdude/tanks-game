#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class BananaPower : public Power{
protected:
	//static ColorValueHolder classColor;

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::string getName() { return BananaPower::getClassName(); }
	static std::string getClassName() { return "banana"; }
	virtual ColorValueHolder getColor() { return BananaPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x99/255.0, 0x99/255.0, 0x33/255.0); } //dull gold

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	BananaPower();
	~BananaPower();
	static Power* factory();
};
