#pragma once
class GodmodeTankPower;

#include "tankpower.h"
#include "godmodepower.h"

class GodmodeTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() {
		return GodmodePower::getClassColor();
	}
	virtual double getColorImportance() override {
		return GodmodePower::getClassColorImportance();
	}

	virtual TankPower* makeDuplicate() { return new GodmodeTankPower(); }
	virtual BulletPower* makeBulletPower();

	GodmodeTankPower();
};
