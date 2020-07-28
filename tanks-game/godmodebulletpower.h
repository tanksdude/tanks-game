#pragma once
class GodmodeBulletPower;

#include "bulletpower.h"
#include "godmodepower.h"

class GodmodeBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return GodmodePower::getClassColor();
	}
	virtual double getColorImportance() const override {
		return GodmodePower::getClassColorImportance();
	}

	virtual BulletPower* makeDuplicate() const override { return new GodmodeBulletPower(); }
	virtual TankPower* makeTankPower() const override;

	GodmodeBulletPower();
};
