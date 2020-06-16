#pragma once
#include "power.h"

class MegaDeathPower : public Power{
public:
	virtual std::vector<std::string> getPowerAttributes() {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() { return MegaDeathPower::getClassName(); }
	static std::string getClassName() { return "megadeath"; }
	virtual ColorValueHolder getColor() { return MegaDeathPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x4F/255.0, 0x3E/255.0, 0x3E/255.0); } //brown-ish

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	MegaDeathPower();
	static Power* factory();
};
