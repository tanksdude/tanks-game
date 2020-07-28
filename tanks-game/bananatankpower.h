#pragma once
class BananaTankPower;

#include "tankpower.h"
#include "bananapower.h"

class BananaTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return BananaPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new BananaTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	BananaTankPower();
};
