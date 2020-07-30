#pragma once
#include "power.h"

class TrackingPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "vanilla-extra", .25f });
		return weights;
	}
	virtual std::vector<std::string> getPowerAttributes() const override {
		//doesn't mix well
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return TrackingPower::getClassName(); }
	static std::string getClassName() { return "tracking"; }
	virtual ColorValueHolder getColor() const override { return TrackingPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0xDD/255.0, 0.25f, 0.25f); } //faded red-orange ("maroon")

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	TrackingPower();
	static Power* factory();
};
