#pragma once
#include "power.h"

class SpeedPower : public Power {
public:
	virtual std::string getName() { return SpeedPower::getClassName(); }
	static std::string getClassName() { return "speed"; }
	virtual ColorValueHolder getColor() { return SpeedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x44/255.0, 0x44/255.0, 1.0f); } //blue

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	SpeedPower();
	static Power* factory();
};
