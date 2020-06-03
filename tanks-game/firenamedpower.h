#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class FireNamedPower : public Power{
protected:
	//static ColorValueHolder classColor;

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::string getName() { return FireNamedPower::getClassName(); }
	static std::string getClassName() { return "fire"; }
	virtual ColorValueHolder getColor() { return FireNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 0.5f, 0.0f); } //orange

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	FireNamedPower();
	~FireNamedPower();
	static Power* factory();
};
