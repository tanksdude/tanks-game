#pragma once
#include "power.h"

class InversionPower : public Power{
public:
	virtual std::vector<std::string> getPowerTypes() {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "dev", .5f });
		weights.insert({ "random-dev", .5f });
		return weights;
	}
	virtual std::vector<std::string> getPowerAttributes() {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() { return InversionPower::getClassName(); }
	static std::string getClassName() { return "inversion"; }
	virtual ColorValueHolder getColor() { return InversionPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(.75f, .75f, .875f); } //silver-teal, I think

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	InversionPower();
	static Power* factory();
};
