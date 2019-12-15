#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class SpeedPower : public Power{
protected:
	//tp = new SpeedTankPower();
	//bp = nullptr;
	//static ColorValueHolder classColor;

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::string getName() { return SpeedPower::getClassName(); }
	static std::string getClassName() { return "speed"; }
	virtual ColorValueHolder getColor() { return SpeedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x44, 0x44, 0xFF); } //blue

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	SpeedPower();
	~SpeedPower();
	static Power* factory();
};