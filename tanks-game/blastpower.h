#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class BlastPower : public Power{
protected:
	//static ColorValueHolder classColor;

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::string getName() { return BlastPower::getClassName(); }
	static std::string getClassName() { return "blast"; }
	virtual ColorValueHolder getColor() { return BlastPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x44/255.0, 0x66/255.0, 0x88/255.0); } //dark blue

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	BlastPower();
	~BlastPower();
	static Power* factory();
};
