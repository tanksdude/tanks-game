#pragma once
#include "power.h"

class GodmodePower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", /*"random-vanilla",*/ "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "vanilla", .5f });
		//weights.insert({ "random-vanilla", .5f });
		weights.insert({ "random", .5f });
		return weights;
	}
	virtual std::vector<std::string> getPowerAttributes() override {
		std::vector<std::string> attributes; //not supposed to mix with other powers
		return attributes;
	}

	virtual std::string getName() { return GodmodePower::getClassName(); }
	static std::string getClassName() { return "godmode"; }
	virtual ColorValueHolder getColor() { return GodmodePower::getClassColor(); }
	static ColorValueHolder getClassColor(); //rainbow!
	virtual double getColorImportance() override { return GodmodePower::getClassColorImportance(); }
	static double getClassColorImportance() { return 1; }

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	GodmodePower();
	static Power* factory();
};
