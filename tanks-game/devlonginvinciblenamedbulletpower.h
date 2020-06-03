#pragma once
class DevLongInvincibleNamedBulletPower;

#include "power.h"
#include "bulletpower.h"
#include "devlonginvinciblenamedpower.h"

class DevLongInvincibleNamedBulletPower : public BulletPower {
protected:

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return DevLongInvincibleNamedPower::getClassColor();
	}

	virtual TankPower* makeTankPower();

	virtual double getOffenseImportance() { return 0; }
	virtual double getOffenseTier(Bullet*) { return 1; }
	virtual double getDefenseImportance() { return 0; }
	virtual double getDefenseTier(Bullet*) { return 1; }

	DevLongInvincibleNamedBulletPower();
};
