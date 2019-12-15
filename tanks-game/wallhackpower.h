#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class WallhackPower : public Power{
protected:

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::string getName() { return WallhackPower::getClassName(); }
	static std::string getClassName() { return "wallhack"; }
	virtual ColorValueHolder getColor() { return WallhackPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xFF, 0xFF, 0x00); } //yellow

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	WallhackPower();
	~WallhackPower();
	static Power* factory();
};