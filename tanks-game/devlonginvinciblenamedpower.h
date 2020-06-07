#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class DevLongInvincibleNamedPower : public Power{
protected:
	//static ColorValueHolder classColor;

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::string getName() { return DevLongInvincibleNamedPower::getClassName(); }
	static std::string getClassName() { return "longinvincible"; }
	virtual ColorValueHolder getColor() { return DevLongInvincibleNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 1.0f, 1.0f); } //white

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	DevLongInvincibleNamedPower();
	~DevLongInvincibleNamedPower();
	static Power* factory();
};
