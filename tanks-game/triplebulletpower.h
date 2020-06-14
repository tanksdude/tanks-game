#pragma once
class TripleBulletPower;

#include "bulletpower.h"
#include "triplepower.h"

class TripleBulletPower : public BulletPower {

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return TriplePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new TripleBulletPower(); }
	virtual TankPower* makeTankPower();

	TripleBulletPower();
};
