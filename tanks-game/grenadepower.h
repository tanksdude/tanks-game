#pragma once
#include "power.h"

class GrenadePower : public Power {
public:
	virtual std::vector<std::string> getPowerAttributes() override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() { return GrenadePower::getClassName(); }
	static std::string getClassName() { return "grenade"; }
	virtual ColorValueHolder getColor() { return GrenadePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xCC/255.0, 1.0f, 0.0f); } //lime green

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	GrenadePower();
	static Power* factory();
};
