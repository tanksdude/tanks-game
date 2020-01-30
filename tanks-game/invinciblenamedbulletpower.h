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

	virtual double getOffenseImportance() { return 0; }
	virtual double getOffenseTier(Bullet*) { return 1; }
	virtual double getDefenseImportance() { return 0; }
	virtual double getDefenseTier(Bullet*) { return 1; }

	InvincibleNamedBulletPower();
};