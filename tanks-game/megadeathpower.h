#pragma once
#include "power.h"

class MegaDeathPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return MegaDeathPower::getClassName(); }
	static std::string getClassName() { return "megadeath"; }
	virtual ColorValueHolder getColor() const override { return MegaDeathPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x4F/255.0, 0x3E/255.0, 0x3E/255.0); } //brown-ish

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	MegaDeathPower();
	static Power* factory();
};
