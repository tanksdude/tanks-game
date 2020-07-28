#pragma once
#include "power.h"

class HomingPower : public Power {
public:
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return HomingPower::getClassName(); }
	static std::string getClassName() { return "homing"; }
	virtual ColorValueHolder getColor() const override { return HomingPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xBB/255.0, 0x66/255.0, 0xAA/255.0); } //really faded purple

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	HomingPower();
	static Power* factory();
};
