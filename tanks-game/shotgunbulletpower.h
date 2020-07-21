#pragma once
class ShotgunBulletPower;

#include "bulletpower.h"
#include "shotgunpower.h"

class ShotgunBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() {
		return ShotgunPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new ShotgunBulletPower(); }
	virtual TankPower* makeTankPower();

	virtual double getBulletSpeedMultiplier() override { return .5; }

	ShotgunBulletPower();
};
