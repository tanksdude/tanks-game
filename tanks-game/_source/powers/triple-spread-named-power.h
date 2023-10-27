#pragma once
//#include "../power.h"
#include "triple-named-power.h"

class TripleSpreadNamedPower : public TripleNamedPower {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual std::string getName() const override { return TripleSpreadNamedPower::getClassName(); }
	static std::string getClassName() { return "triple_spread"; }
	virtual ColorValueHolder getColor() const override { return TripleSpreadNamedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.85f, 0.85f, 0.60f); } //beige but a bit brighter

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	TripleSpreadNamedPower();
	static Power* factory();
};



class TripleSpreadNamedTankPower : public TripleNamedTankPower {
public:
	virtual ColorValueHolder getColor() const override {
		return TripleSpreadNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new TripleSpreadNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool addsExtraShootingPoints = true;
	virtual std::vector<std::pair<double, double>>* addExtraShootingPoints() const override;

	TripleSpreadNamedTankPower();
};



class TripleSpreadNamedBulletPower : public TripleNamedBulletPower {
public:
	virtual ColorValueHolder getColor() const override {
		return TripleSpreadNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new TripleSpreadNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	TripleSpreadNamedBulletPower();
};
