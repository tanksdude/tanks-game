#pragma once
#include "power.h"

class MultishotPower : public Power {
	friend class MultishotTankPower;

protected:
	static const int bulletCount = 8;

public:
	virtual std::string getName() const override { return MultishotPower::getClassName(); }
	static std::string getClassName() { return "multishot"; }
	virtual ColorValueHolder getColor() const override { return MultishotPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0.5f, 0.0f, 1.0f); } //purple

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	MultishotPower();
	static Power* factory();
};



class MultishotTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return MultishotPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new MultishotTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool addsShootingPoints = true;
	virtual void addShootingPoints(Tank*, std::vector<CannonPoint>*) override;
	//bool overridesAddShootingPoints = false;
	//bool addShootingPointsCanWorkWithOthers = true;
	//bool addShootingPointsCanOnlyWorkIndividually = false;

	virtual double getTankFiringRateMultiplier() const override { return .5; }

	MultishotTankPower();
};



class MultishotBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return MultishotPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new MultishotBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	MultishotBulletPower();
};
