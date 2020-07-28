#pragma once
class DevLongInvincibleNamedTankPower;

#include "tankpower.h"
#include "devlonginvinciblenamedpower.h"

class DevLongInvincibleNamedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return DevLongInvincibleNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new DevLongInvincibleNamedTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankMaxSpeedMultiplier() const override { return .5; }
	virtual double getTankAccelerationMultiplier() const override { return .5; }
	
	virtual double getOffenseImportance() const override { return 0; }
	virtual double getOffenseTier(Tank*) const override { return 0; }
	virtual double getDefenseImportance() const override { return 0; }
	virtual double getDefenseTier(Tank*) const override { return 2.5; }

	DevLongInvincibleNamedTankPower();
};
