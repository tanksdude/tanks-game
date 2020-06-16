#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class GrenadePower : public Power{
protected:
	//static ColorValueHolder classColor;

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::vector<std::string> getPowerAttributes() {
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
	~GrenadePower();
	static Power* factory();
};
