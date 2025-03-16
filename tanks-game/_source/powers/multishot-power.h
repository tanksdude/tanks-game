#pragma once
#include "../power.h"

class MultishotPower : public Power {
public: //tank stuff
	static const int bulletCount = 8;

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "supermix", "supermix-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual std::string getName() const override { return MultishotPower::getClassName(); }
	static std::string getClassName() { return "multishot"; }
	virtual ColorValueHolder getColor() const override { return MultishotPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.5f, 0.0f, 1.0f); } //purple //JS: #8800FF

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	MultishotPower();
	static Power* factory();
};



class MultishotTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override { return MultishotPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new MultishotTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool addsShootingPoints = true;
	virtual std::vector<double>* addShootingPoints() const override;

	virtual double getTankFiringRateMultiplier() const override { return .5; }

	MultishotTankPower();
};



class MultishotBulletPower : public BulletPower {
public:
	virtual ColorValueHolder getColor() const override { return MultishotPower::getClassColor(); }
	virtual std::string getColorIdentifier() const override { return MultishotPower::getClassName(); }

	virtual BulletPower* makeDuplicate() const override { return new MultishotBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	//virtual double getBulletSpeedMultiplier() const override { return .5; } //JS

	MultishotBulletPower();
};
