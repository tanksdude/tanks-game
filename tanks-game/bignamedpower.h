#pragma once
#include "power.h"

class BigNamedPower : public Power{
public:
	virtual std::string getName() { return BigNamedPower::getClassName(); }
	static std::string getClassName() { return "big"; }
	virtual ColorValueHolder getColor() { return BigNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 0.0f, 0.0f); } //red

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	BigNamedPower();
	static Power* factory();
};
