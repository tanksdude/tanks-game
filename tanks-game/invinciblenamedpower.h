#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class InvincibleNamedPower : public Power{
protected:
	//static ColorValueHolder classColor;

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::vector<std::string> getPowerAttributes() {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() { return InvincibleNamedPower::getClassName(); }
	static std::string getClassName() { return "invincible"; }
	virtual ColorValueHolder getColor() { return InvincibleNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 1.0f, 1.0f); } //white

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	InvincibleNamedPower();
	~InvincibleNamedPower();
	static Power* factory();
};
