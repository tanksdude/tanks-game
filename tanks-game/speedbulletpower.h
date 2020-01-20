#pragma once
class SpeedBulletPower;

#include "power.h"
#include "bulletpower.h"
#include "speedpower.h"

class SpeedBulletPower : public BulletPower {

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual void tick() { return; }
	virtual ColorValueHolder getColor() {
		return SpeedPower::getClassColor();
	}

	virtual TankPower* makeTankPower();

	SpeedBulletPower();
};