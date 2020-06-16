#pragma once
class OldBounceTankPower;

#include "tankpower.h"
#include "oldbouncepower.h"

class OldBounceTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return OldBouncePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new OldBounceTankPower(); }
	virtual BulletPower* makeBulletPower();

	virtual double getTankAccelerationMultiplier() { return .5; }
	virtual double getTankRadiusMultiplier() { return .5; } //should this stack?
	virtual double getFiringRateMultiplier() { return .5; }

	OldBounceTankPower();
};
