#pragma once
#include "power.h"

class TriplePower : public Power {
public:
	virtual std::vector<std::string> getPowerAttributes() override {
		//might want to change how this power works (need to adjust cannonpoints)
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() { return TriplePower::getClassName(); }
	static std::string getClassName() { return "triple"; }
	virtual ColorValueHolder getColor() { return TriplePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xCC/255.0, 0xCC/255.0, 0.5f); } //beige

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	TriplePower();
	static Power* factory();
};
