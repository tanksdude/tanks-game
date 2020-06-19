#pragma once
class InversionBulletPower;

#include "bulletpower.h"
#include "inversionpower.h"

class InversionBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return InversionPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new InversionBulletPower(); }
	virtual TankPower* makeTankPower();

	InversionBulletPower();
};
