#pragma once
#include "power.h"

class SpeedPower : public Power {
public:
	virtual std::string getName() const override { return SpeedPower::getClassName(); }
	static std::string getClassName() { return "speed"; }
	virtual ColorValueHolder getColor() const override { return SpeedPower::getClassColor(); }
	static ColorValueHolder getClassColor() { return ColorValueHolder(0x44/255.0, 0x44/255.0, 1.0f); } //blue

	virtual TankPower* makeTankPower() const override;
	virtual BulletPower* makeBulletPower() const override;
	//virtual HazardPower* makeHazardPower() const override;

	SpeedPower();
	static Power* factory();
};



class SpeedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return SpeedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new SpeedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankMaxSpeedMultiplier() const override;
	//bool tankMaxSpeedStacks = true;

	SpeedTankPower();
};



class SpeedBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return SpeedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new SpeedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	SpeedBulletPower();
};
