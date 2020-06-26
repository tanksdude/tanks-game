#pragma once
#include "power.h"

class OldBigNamedPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() override {
		std::vector<std::string> types = std::vector<std::string>{ "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "old", .5f });
		weights.insert({ "random-old", .5f });
		return weights;
	}

	virtual std::string getName() { return OldBigNamedPower::getClassName(); }
	static std::string getClassName() { return "oldbig"; }
	virtual ColorValueHolder getColor() { return OldBigNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 0.0f, 0.0f); } //red

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	OldBigNamedPower();
	static Power* factory();
};
