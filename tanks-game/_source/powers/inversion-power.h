#pragma once
#include "../power.h"

class InversionPower : public Power {
public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual std::string getName() const override { return InversionPower::getClassName(); }
	static std::string getClassName() { return "inversion"; }
	virtual std::string getIdentifier() const override { return InversionPower::getClassIdentifier(); }
	static std::string getClassIdentifier() { return InversionPower::getClassName(); }
	virtual ColorValueHolder getColor() const override { return InversionPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.75f, 0.75f, 0.875f); } //silver-teal, I think

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	InversionPower();
	static Power* factory();
};



class InversionTankPower : public TankPower {
public:
	virtual std::string getIdentifier() const override { return InversionPower::getClassIdentifier(); }
	virtual ColorValueHolder getColor() const override { return InversionPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new InversionTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankTurningIncrementMultiplier() const override { return -1; }
	//bool tankTurningIncrementStacks = true;

	InversionTankPower();
};



class InversionBulletPower : public BulletPower {
public:
	virtual std::string getIdentifier() const override { return InversionPower::getClassIdentifier(); }
	virtual ColorValueHolder getColor() const override { return InversionPower::getClassColor(); }

	virtual BulletPower* makeDuplicate() const override { return new InversionBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	InversionBulletPower();
};
