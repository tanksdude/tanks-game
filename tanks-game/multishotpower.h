#pragma once
#include "power.h"

class MultishotPower : public Power {
	friend class MultishotTankPower;
protected:
	static const short bulletConstant = 8;

public:
	virtual std::string getName() { return MultishotPower::getClassName(); }
	static std::string getClassName() { return "multishot"; }
	virtual ColorValueHolder getColor() { return MultishotPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.5f, 0.0f, 1.0f); } //purple

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	MultishotPower();
	static Power* factory();
};
