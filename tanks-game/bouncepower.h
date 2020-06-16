#pragma once
#include "power.h"

class BouncePower : public Power{
public:
	virtual std::vector<std::string> getPowerAttributes() {
		//I'm very conflicted on this one: while it does handle stacking (like all powers should), it's not recommended; it can mix with others, but not to great results
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() { return BouncePower::getClassName(); }
	static std::string getClassName() { return "bounce"; }
	virtual ColorValueHolder getColor() { return BouncePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 0.0f, .75f); } //pink

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	BouncePower();
	static Power* factory();
};
