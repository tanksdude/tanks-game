#pragma once
#include "power.h"

class WallhackPower : public Power{
public:
	virtual std::vector<std::string> getPowerAttributes() {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() { return WallhackPower::getClassName(); }
	static std::string getClassName() { return "wallhack"; }
	virtual ColorValueHolder getColor() { return WallhackPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 1.0f, 0.0f); } //yellow

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	WallhackPower();
	static Power* factory();
};
