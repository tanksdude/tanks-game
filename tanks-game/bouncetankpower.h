#pragma once
class BounceTankPower;

#include "tankpower.h"
#include "bouncepower.h"

class BounceTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BouncePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new BounceTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	BounceTankPower();
};
