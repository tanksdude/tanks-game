#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class MultishotPower : public Power {
	friend class MultishotTankPower;
protected:
	static const short bulletConstant = 8;

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::string getName() { return MultishotPower::getClassName(); }
	static std::string getClassName() { return "multishot"; }
	virtual ColorValueHolder getColor() { return MultishotPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(.5f, 0.f, 1.f); } //purple

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	MultishotPower();
	~MultishotPower();
	static Power* factory();
};