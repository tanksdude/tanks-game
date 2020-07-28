#pragma once
class MegaDeathTankPower;

#include "tankpower.h"
#include "megadeathpower.h"

class MegaDeathTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return MegaDeathPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new MegaDeathTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankFiringRateMultiplier() const override { return 4; }

	MegaDeathTankPower();
};
