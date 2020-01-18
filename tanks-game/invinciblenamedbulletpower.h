#pragma once
class InvincibleNamedBulletPower;

#include "power.h"
#include "bulletpower.h"
#include "invinciblenamedpower.h"

class InvincibleNamedBulletPower : public BulletPower {
protected:

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual void tick() { return; }
	virtual ColorValueHolder getColor() {
		return InvincibleNamedPower::getClassColor();
	}

	virtual TankPower* makeTankPower();

	//virtual double getOffenseTier() { return 0; }
	//virtual double getOffenseValue() { return 0; }
	//virtual double getDefenseTier() { return 0; }
	//virtual double getDefenseValue() { return 0; }

	InvincibleNamedBulletPower();
};