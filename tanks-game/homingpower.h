#pragma once
#include "power.h"

class HomingPower : public Power{
public:
	virtual std::vector<std::string> getPowerAttributes() {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() { return HomingPower::getClassName(); }
	static std::string getClassName() { return "homing"; }
	virtual ColorValueHolder getColor() { return HomingPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xBB/255.0, 0x66/255.0, 0xAA/255.0); } //really faded purple

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	HomingPower();
	static Power* factory();
};
