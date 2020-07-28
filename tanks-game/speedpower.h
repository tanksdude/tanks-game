#pragma once
#include "power.h"

class SpeedPower : public Power {
public:
	virtual std::string getName() const override { return SpeedPower::getClassName(); }
	static std::string getClassName() { return "speed"; }
	virtual ColorValueHolder getColor() const override { return SpeedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x44/255.0, 0x44/255.0, 1.0f); } //blue

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	SpeedPower();
	static Power* factory();
};
