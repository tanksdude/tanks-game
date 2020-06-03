#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class MegaDeathPower : public Power{
protected:
	//static ColorValueHolder classColor;

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::string getName() { return MegaDeathPower::getClassName(); }
	static std::string getClassName() { return "megadeath"; }
	virtual ColorValueHolder getColor() { return MegaDeathPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x4F/255.0, 0x3E/255.0, 0x3E/255.0); } //brown-ish

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	MegaDeathPower();
	~MegaDeathPower();
	static Power* factory();
};
