#pragma once
#include "power.h"

class DevLongInvincibleNamedPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() override {
		std::vector<std::string> types = std::vector<std::string>{ "dev" }; //no random-dev
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "dev", 0.0f }); //this should not appear, like, ever
		return weights;
	}
	virtual std::vector<std::string> getPowerAttributes() override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() { return DevLongInvincibleNamedPower::getClassName(); }
	static std::string getClassName() { return "longinvincible"; }
	virtual ColorValueHolder getColor() { return DevLongInvincibleNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 1.0f, 1.0f); } //white

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	DevLongInvincibleNamedPower();
	static Power* factory();
};
