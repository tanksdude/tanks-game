#pragma once
class OldBounceTankPower;

#include "tankpower.h"
#include "oldbouncepower.h"

class OldBounceTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return OldBouncePower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new OldBounceTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankAccelerationMultiplier() const override { return .5; }
	virtual double getTankRadiusMultiplier() const override { return .5; } //should this stack?
	virtual double getTankFiringRateMultiplier() const override { return .5; }

	OldBounceTankPower();
};
