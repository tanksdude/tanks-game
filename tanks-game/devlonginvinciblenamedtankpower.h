#pragma once
class DevLongInvincibleNamedTankPower;

#include "tankpower.h"
#include "devlonginvinciblenamedpower.h"

class DevLongInvincibleNamedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() {
		return DevLongInvincibleNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new DevLongInvincibleNamedTankPower(); }
	virtual BulletPower* makeBulletPower();

	virtual double getTankMaxSpeedMultiplier() override { return .5; }
	virtual double getTankAccelerationMultiplier() override { return .5; }
	
	virtual double getOffenseImportance() override { return 0; }
	virtual double getOffenseTier(Tank*) override { return 0; }
	virtual double getDefenseImportance() override { return 0; }
	virtual double getDefenseTier(Tank*) override { return 2.5; }

	DevLongInvincibleNamedTankPower();
};
