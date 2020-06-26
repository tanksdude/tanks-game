#pragma once
class InvincibleNamedBulletPower;

#include "bulletpower.h"
#include "invinciblenamedpower.h"

class InvincibleNamedBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() {
		return InvincibleNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new InvincibleNamedBulletPower(); }
	virtual TankPower* makeTankPower();

	virtual double getOffenseImportance() override { return 0; }
	virtual double getOffenseTier(Bullet*) override { return 1; }
	virtual double getDefenseImportance() override { return 0; }
	virtual double getDefenseTier(Bullet*) override { return 1; }

	InvincibleNamedBulletPower();
};
