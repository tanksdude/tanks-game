#pragma once
class OldBounceTankPower;

#include "tankpower.h"
#include "oldbouncepower.h"

class OldBounceTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() {
		return OldBouncePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new OldBounceTankPower(); }
	virtual BulletPower* makeBulletPower();

	virtual double getTankAccelerationMultiplier() override { return .5; }
	virtual double getTankRadiusMultiplier() override { return .5; } //should this stack?
	virtual double getTankFiringRateMultiplier() override { return .5; }

	OldBounceTankPower();
};
