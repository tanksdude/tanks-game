#pragma once
class GodmodeTankPower;

#include "tankpower.h"
#include "godmodepower.h"

class GodmodeTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent);
	virtual void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return GodmodePower::getClassColor();
	}
	virtual double getColorImportance() {
		return GodmodePower::getClassColorImportance();
	}

	virtual TankPower* makeDuplicate() { return new GodmodeTankPower(); }
	virtual BulletPower* makeBulletPower();

	GodmodeTankPower();
};
