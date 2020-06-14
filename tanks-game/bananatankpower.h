#pragma once
class BananaTankPower;

#include "tankpower.h"
#include "bananapower.h"

class BananaTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return BananaPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new BananaTankPower(); }
	virtual BulletPower* makeBulletPower();

	BananaTankPower();
};
