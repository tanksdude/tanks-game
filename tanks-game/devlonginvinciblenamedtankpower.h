#pragma once
class DevLongInvincibleNamedTankPower;

#include "tankpower.h"
#include "devlonginvinciblenamedpower.h"

class DevLongInvincibleNamedTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return DevLongInvincibleNamedPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new DevLongInvincibleNamedTankPower(); }
	virtual BulletPower* makeBulletPower();

	virtual double getTankMaxSpeedMultiplier() { return .5; }
	virtual double getTankAccelerationMultiplier() { return .5; }
	
	virtual double getOffenseImportance() { return 0; }
	virtual double getOffenseTier(Tank*) { return 0; }
	virtual double getDefenseImportance() { return 0; }
	virtual double getDefenseTier(Tank*) { return 2.5; }

	DevLongInvincibleNamedTankPower();
};
