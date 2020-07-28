#pragma once
#include "power.h"

class WallhackPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "vanilla", .5f });
		weights.insert({ "random-vanilla", .5f });
		weights.insert({ "random", .5f });
		return weights;
	}
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return WallhackPower::getClassName(); }
	static std::string getClassName() { return "wallhack"; }
	virtual ColorValueHolder getColor() const override { return WallhackPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 1.0f, 0.0f); } //yellow

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	WallhackPower();
	static Power* factory();
};
