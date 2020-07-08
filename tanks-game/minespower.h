#pragma once
#include "power.h"

//TODO: this should be renamed (but probably won't...)
class MinesPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() override {
		std::unordered_map<std::string, float> weights;
		weights.insert({ "vanilla-extra", .25f });
		return weights;
	}
	virtual std::vector<std::string> getPowerAttributes() override {
		//conflicted on this one, like bounce, though leaning much more towards "doesn't stack" than "does stack"
		//also somewhat conflicted on mixing because it just doesn't mix well
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() { return MinesPower::getClassName(); }
	static std::string getClassName() { return "mines"; }
	virtual ColorValueHolder getColor() { return MinesPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0, 0, 0); } //black, so it's hard to see where the tank is pointed
	virtual double getColorImportance() override { return MinesPower::getClassColorImportance(); }
	static double getClassColorImportance() { return .5; } //godmode can overpower this

	virtual TankPower* makeTankPower();
	virtual BulletPower* makeBulletPower();
	//virtual HazardPower* makeHazardPower();

	MinesPower();
	static Power* factory();
};
