#pragma once
class GodmodeBulletPower;

#include "bulletpower.h"
#include "godmodepower.h"

class GodmodeBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return GodmodePower::getClassColor();
	}
	virtual double getColorImportance() {
		return GodmodePower::getClassColorImportance();
	}

	virtual BulletPower* makeDuplicate() { return new GodmodeBulletPower(); }
	virtual TankPower* makeTankPower();

	GodmodeBulletPower();
};
