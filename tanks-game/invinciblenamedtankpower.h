#pragma once
class InvincibleNamedTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
#include "invinciblenamedpower.h"

class InvincibleNamedTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return InvincibleNamedPower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	virtual double getOffenseImportance() { return 0; }
	virtual double getOffenseTier() { return 0; }
	virtual double getDefenseImportance() { return 0; }
	virtual double getDefenseTier() { return 2.5; }

	InvincibleNamedTankPower();
};