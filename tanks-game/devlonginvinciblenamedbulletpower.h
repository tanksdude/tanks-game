#pragma once
class DevLongInvincibleNamedBulletPower;

#include "bulletpower.h"
#include "devlonginvinciblenamedpower.h"

class DevLongInvincibleNamedBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() {
		return DevLongInvincibleNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new DevLongInvincibleNamedBulletPower(); }
	virtual TankPower* makeTankPower();

	virtual double getOffenseImportance() override { return 0; }
	virtual double getOffenseTier(Bullet*) override { return 1; }
	virtual double getDefenseImportance() override { return 0; }
	virtual double getDefenseTier(Bullet*) override { return 1; }

	DevLongInvincibleNamedBulletPower();
};
