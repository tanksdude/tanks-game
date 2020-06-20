#pragma once
class MultishotBulletPower;

#include "bulletpower.h"
#include "multishotpower.h"

class MultishotBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() {
		return MultishotPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new MultishotBulletPower(); }
	virtual TankPower* makeTankPower();

	MultishotBulletPower();
};
