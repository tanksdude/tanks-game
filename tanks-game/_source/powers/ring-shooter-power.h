#pragma once
#include "../power.h"

class RingShooterPower : public Power {
public: //tank stuff
	static const int bulletCount = 8;

public:
	virtual std::vector<std::string> getPowerTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual std::vector<std::string> getPowerAttributes() const override {
		std::vector<std::string> attributes = std::vector<std::string>{ "mix" };
		return attributes;
	}

	virtual std::string getName() const override { return RingShooterPower::getClassName(); }
	static std::string getClassName() { return "ring_shooter"; }
	virtual ColorValueHolder getColor() const override { return RingShooterPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.6f, 0.1f, 1.0f); } //purple but a bit brighter

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;

	RingShooterPower();
	static Power* factory();
};



class RingShooterTankPower : public TankPower {
public:
	virtual ColorValueHolder getColor() const override { return RingShooterPower::getClassColor(); }

	virtual TankPower* makeDuplicate() const override { return new RingShooterTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool addsExtraShootingPoints = true;
	virtual std::vector<std::pair<double, double>>* addExtraShootingPoints() const override;

	virtual double getTankFiringRateMultiplier() const override { return .5; }

	RingShooterTankPower();
};



class RingShooterBulletPower : public BulletPower {
public:
	virtual ColorValueHolder getColor() const override { return RingShooterPower::getClassColor(); }

	virtual BulletPower* makeDuplicate() const override { return new RingShooterBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	virtual double getBulletSpeedMultiplier() const override { return .5; }

	RingShooterBulletPower();
};
