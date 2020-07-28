#pragma once
#include "power.h"

class BouncePower : public Power {
public:
	/*
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
	*/
	virtual std::vector<std::string> getPowerAttributes() const override {
		//I'm very conflicted on this one: while it does handle stacking (like all powers should), it's not recommended; it can mix with others, but not to great results
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return BouncePower::getClassName(); }
	static std::string getClassName() { return "bounce"; }
	virtual ColorValueHolder getColor() const override { return BouncePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 0.0f, .75f); } //pink

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	BouncePower();
	static Power* factory();
};
