#pragma once
#include "../power.h"

class DevWeirdExtraCannonsPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	//virtual std::vector<std::string> getPowerAttributes() const override;

	virtual std::string getName() const override { return DevWeirdExtraCannonsPower::getClassName(); }
	static std::string getClassName() { return "weird_extra_cannons"; }
	virtual ColorValueHolder getColor() const override { return DevWeirdExtraCannonsPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.625f, 0.625f, 0.625f); } //light-ish gray

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	DevWeirdExtraCannonsPower();
	static Power* factory();
};



class DevWeirdExtraCannonsTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override {
		return DevWeirdExtraCannonsPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new DevWeirdExtraCannonsTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool addsShootingPoints = true;
	virtual std::vector<double>* addShootingPoints() const override;

	virtual double getTankFiringRateMultiplier() const override { return .5; }

	DevWeirdExtraCannonsTankPower();
};



class DevWeirdExtraCannonsBulletPower : public BulletPower {
public:
	virtual ColorValueHolder getColor() const override {
		return DevWeirdExtraCannonsPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new DevWeirdExtraCannonsBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	DevWeirdExtraCannonsBulletPower();
};
