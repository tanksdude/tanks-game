#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class BigNamedPower : public Power{
protected:
	//static ColorValueHolder classColor;

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::string getName() { return BigNamedPower::getClassName(); }
	static std::string getClassName() { return "big"; }
	virtual ColorValueHolder getColor() { return BigNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 0.0f, 0.0f); } //red

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	BigNamedPower();
	~BigNamedPower();
	static Power* factory();
};
