#pragma once
class InvincibleNamedTankPower;

#include "tankpower.h"
#include "invinciblenamedpower.h"

class InvincibleNamedTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return InvincibleNamedPower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	virtual double getTankMaxSpeedMultiplier() { return .5; }
	virtual double getTankAccelerationMultiplier() { return .5; }
	
	virtual double getOffenseImportance() { return 0; }
	virtual double getOffenseTier(Tank*) { return 0; }
	virtual double getDefenseImportance() { return 0; }
	virtual double getDefenseTier(Tank*) { return 2.5; } // this is not DESTRUCTION_TIER+.5 in case someone wants to have a higher destruction tier

	InvincibleNamedTankPower();
};
