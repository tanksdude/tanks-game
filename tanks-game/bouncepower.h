#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class BouncePower : public Power{
protected:
	//static ColorValueHolder classColor;

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::string getName() { return BouncePower::getClassName(); }
	static std::string getClassName() { return "bounce"; }
	virtual ColorValueHolder getColor() { return BouncePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 0x00/255.0, 0xCC/255.0); } //pink

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	BouncePower();
	~BouncePower();
	static Power* factory();
};
