#pragma once
class TrackingTankPower;

#include "tankpower.h"
#include "trackingpower.h"

class TrackingTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return TrackingPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new TrackingTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	virtual double getTankMaxSpeedMultiplier() const override { return .5; }
	virtual double getTankAccelerationMultiplier() const override { return .5; }
	virtual double getTankFiringRateMultiplier() const override { return 2; }

	TrackingTankPower();
};
