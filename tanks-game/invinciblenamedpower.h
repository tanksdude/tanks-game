#pragma once
#include "power.h"

class InvincibleNamedPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "vanilla", .5f });
		weights.insert({ "random-vanilla", .5f });
		weights.insert({ "random", .5f });
		return weights;
	}
	virtual std::vector<std::string> getPowerAttributes() override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() { return InvincibleNamedPower::getClassName(); }
	static std::string getClassName() { return "invincible"; }
	virtual ColorValueHolder getColor() { return InvincibleNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 1.0f, 1.0f); } //white

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	InvincibleNamedPower();
	static Power* factory();
};
