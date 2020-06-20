#pragma once
#include "power.h"

class BananaPower : public Power {
public:
	virtual std::string getName() { return BananaPower::getClassName(); }
	static std::string getClassName() { return "banana"; }
	virtual ColorValueHolder getColor() { return BananaPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x99/255.0, 0x99/255.0, 0x33/255.0); } //dull gold

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	BananaPower();
	static Power* factory();
};
