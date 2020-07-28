#pragma once
#include "power.h"

class BlastPower : public Power {
	//called mega-blast in JS Tanks
public:
	//TODO: move to vanilla-extra?
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "vanilla", .5f });
		weights.insert({ "random-vanilla", .25f });
		weights.insert({ "random", .25f });
		return weights;
	}
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return BlastPower::getClassName(); }
	static std::string getClassName() { return "blast"; }
	virtual ColorValueHolder getColor() const override { return BlastPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x44/255.0, 0x66/255.0, 0x88/255.0); } //dark blue

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	BlastPower();
	static Power* factory();
};
