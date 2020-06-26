#pragma once
#include "power.h"

class OldBouncePower : public Power {
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
	virtual std::vector<std::string> getPowerAttributes() override {
		//match with BouncePower
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() { return OldBouncePower::getClassName(); }
	static std::string getClassName() { return "oldbounce"; }
	virtual ColorValueHolder getColor() { return OldBouncePower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(1.0f, 0.0f, 0xCC/255.0); } //pink

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	OldBouncePower();
	static Power* factory();
};
