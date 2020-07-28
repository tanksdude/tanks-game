#pragma once
class ShotgunBulletPower;

#include "bulletpower.h"
#include "shotgunpower.h"

class ShotgunBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return ShotgunPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new ShotgunBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	virtual double getBulletSpeedMultiplier() const override { return .5; }

	ShotgunBulletPower();
};
