#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class HomingPower : public Power{
protected:
	//static ColorValueHolder classColor;

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::string getName() { return HomingPower::getClassName(); }
	static std::string getClassName() { return "homing"; }
	virtual ColorValueHolder getColor() { return HomingPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xBB/255.0, 0x66/255.0, 0xAA/255.0); } //really faded purple

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	HomingPower();
	~HomingPower();
	static Power* factory();
};
