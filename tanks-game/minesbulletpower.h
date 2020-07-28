#pragma once
class MinesBulletPower;

#include "bulletpower.h"
#include "minespower.h"

class MinesBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return MinesPower::getClassColor();
	}
	virtual double getColorImportance() const override {
		return MinesPower::getClassColorImportance();
	}

	virtual BulletPower* makeDuplicate() const override { return new MinesBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	virtual double getBulletSpeedMultiplier() const override { return 0; }
	//bool bulletSpeedStacks = false; //true?
	virtual double getBulletAcceleration() const override { return 0; } //TODO: probably need importance values on all of these

	MinesBulletPower();
};
