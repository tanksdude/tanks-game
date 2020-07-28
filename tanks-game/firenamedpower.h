#pragma once
#include "power.h"

class FireNamedPower : public Power {
public:
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return FireNamedPower::getClassName(); }
	static std::string getClassName() { return "fire"; }
	virtual ColorValueHolder getColor() const override { return FireNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 0.5f, 0.0f); } //orange

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	FireNamedPower();
	static Power* factory();
};
