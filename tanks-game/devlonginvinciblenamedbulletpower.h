#pragma once
class DevLongInvincibleNamedBulletPower;

#include "bulletpower.h"
#include "devlonginvinciblenamedpower.h"

class DevLongInvincibleNamedBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return DevLongInvincibleNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new DevLongInvincibleNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	virtual double getOffenseImportance() const override { return 0; }
	virtual double getOffenseTier(Bullet*) const override { return 1; }
	virtual double getDefenseImportance() const override { return 0; }
	virtual double getDefenseTier(Bullet*) const override { return 1; }

	DevLongInvincibleNamedBulletPower();
};
