#pragma once
#include "bulletpower.h"
#include "multishotpower.h"

class MultishotBulletPower : public BulletPower {

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return MultishotPower::getClassColor();
	}

	virtual TankPower* makeTankPower();

	MultishotBulletPower();
};
