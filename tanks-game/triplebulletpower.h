#pragma once
class TripleBulletPower;

#include "power.h"
#include "bulletpower.h"
#include "triplepower.h"

class TripleBulletPower : public BulletPower {

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return TriplePower::getClassColor();
	}

	virtual TankPower* makeTankPower();

	TripleBulletPower();
};
