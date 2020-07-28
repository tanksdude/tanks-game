#pragma once
class GodmodeTankPower;

#include "tankpower.h"
#include "godmodepower.h"

class GodmodeTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return GodmodePower::getClassColor();
	}
	virtual double getColorImportance() const override {
		return GodmodePower::getClassColorImportance();
	}

	virtual TankPower* makeDuplicate() const override { return new GodmodeTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	GodmodeTankPower();
};
