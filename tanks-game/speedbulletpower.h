#pragma once
class SpeedBulletPower;

#include "bulletpower.h"
#include "speedpower.h"

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
