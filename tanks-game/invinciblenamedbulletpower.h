#pragma once
class InvincibleNamedBulletPower;

#include "bulletpower.h"
#include "invinciblenamedpower.h"

class InvincibleNamedBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return InvincibleNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new InvincibleNamedBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	virtual double getOffenseImportance() const override { return 0; }
	virtual double getOffenseTier(Bullet*) const override { return 1; }
	virtual double getDefenseImportance() const override { return 0; }
	virtual double getDefenseTier(Bullet*) const override { return 1; }

	InvincibleNamedBulletPower();
};
