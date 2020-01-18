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

	//virtual double getOffenseTier();
	//virtual double getOffenseValue();
	//virtual double getDefenseTier();
	//virtual double getDefenseValue();

	InvincibleNamedTankPower();
};