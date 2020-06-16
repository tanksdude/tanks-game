#pragma once
#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"

class OldBouncePower : public Power{
protected:
	//static ColorValueHolder classColor;

public:
	const static bool canBeInARandomLevel;
	//bool getCanBeMixed() { return true; }

	virtual std::vector<std::string> getPowerTypes() {
		std::vector<std::string> types = std::vector<std::string>{ "old", "random-old" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "old", .5f });
		weights.insert({ "random-old", .5f });
		return weights;
	}
	virtual std::vector<std::string> getPowerAttributes() {
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
	~OldBouncePower();
	static Power* factory();
};
