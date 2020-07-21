#pragma once
#include "power.h"

class ShotgunPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "vanilla-extra", .5f });
		weights.insert({ "random", .25f });
		return weights;
	}
	virtual std::vector<std::string> getPowerAttributes() override {
		//if adjusting triple, also adjust this
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() { return ShotgunPower::getClassName(); }
	static std::string getClassName() { return "shotgun"; }
	virtual ColorValueHolder getColor() { return ShotgunPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.0f, 1.0f, 0.25f); } //green

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	ShotgunPower();
	static Power* factory();
};
