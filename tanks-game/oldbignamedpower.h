#pragma once
#include "power.h"

class OldBigNamedPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "old", .5f });
		weights.insert({ "random-old", .5f });
		return weights;
	}

	virtual std::string getName() const override { return OldBigNamedPower::getClassName(); }
	static std::string getClassName() { return "oldbig"; }
	virtual ColorValueHolder getColor() const override { return OldBigNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 0.0f, 0.0f); } //red

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	OldBigNamedPower();
	static Power* factory();
};
