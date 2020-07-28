#pragma once
#include "power.h"

class BananaPower : public Power {
public:
	virtual std::string getName() const override { return BananaPower::getClassName(); }
	static std::string getClassName() { return "banana"; }
	virtual ColorValueHolder getColor() const override { return BananaPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x99/255.0, 0x99/255.0, 0x33/255.0); } //dull gold

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	BananaPower();
	static Power* factory();
};
