#pragma once
class InvincibleNamedTankPower;

#include "tankpower.h"
#include "invinciblenamedpower.h"

class InvincibleNamedTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() {
		return InvincibleNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new InvincibleNamedTankPower(); }
	virtual BulletPower* makeBulletPower();

	virtual double getTankMaxSpeedMultiplier() override { return .5; }
	virtual double getTankAccelerationMultiplier() override { return .5; }
	
	virtual double getOffenseImportance() override { return 0; }
	virtual double getOffenseTier(Tank*) override { return 0; }
	virtual double getDefenseImportance() override { return 0; }
	virtual double getDefenseTier(Tank*) override { return 2.5; } // this is not DESTRUCTION_TIER+.5 in case someone wants to have a higher destruction tier

	InvincibleNamedTankPower();
};
