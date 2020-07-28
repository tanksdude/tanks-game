#pragma once
#include "power.h"

class GodmodePower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", /*"random-vanilla",*/ "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "vanilla", .5f });
		//weights.insert({ "random-vanilla", .5f });
		weights.insert({ "random", .5f });
		return weights;
	}
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes; //not supposed to mix with other powers
		return attributes;
	}

	virtual std::string getName() const override { return GodmodePower::getClassName(); }
	static std::string getClassName() { return "godmode"; }
	virtual ColorValueHolder getColor() const override { return GodmodePower::getClassColor(); }
	static ColorValueHolder getClassColor(); //rainbow!
	virtual double getColorImportance() const override { return GodmodePower::getClassColorImportance(); }
	static double getClassColorImportance() { return 1; }

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	GodmodePower();
	static Power* factory();
};
