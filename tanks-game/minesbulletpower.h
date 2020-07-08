#pragma once
class MinesBulletPower;

#include "bulletpower.h"
#include "minespower.h"

class MinesBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() {
		return MinesPower::getClassColor();
	}
	virtual double getColorImportance() override {
		return MinesPower::getClassColorImportance();
	}

	virtual BulletPower* makeDuplicate() { return new MinesBulletPower(); }
	virtual TankPower* makeTankPower();

	virtual double getBulletSpeedMultiplier() override { return 0; }
	//bool bulletSpeedStacks = false; //true?
	virtual double getBulletAcceleration() override { return 0; } //TODO: probably need importance values on all of these

	MinesBulletPower();
};
