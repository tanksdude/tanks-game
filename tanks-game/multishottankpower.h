#pragma once
class MultishotTankPower;

#include "tankpower.h"
#include "multishotpower.h"

class MultishotTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() {
		return MultishotPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new MultishotTankPower(); }
	virtual BulletPower* makeBulletPower();

	//bool addsShootingPoints = true;
	virtual void addShootingPoints(Tank*, std::vector<CannonPoint>*) override;
	//bool overridesAddShootingPoints = false;
	//bool addShootingPointsCanWorkWithOthers = true;
	//bool addShootingPointsCanOnlyWorkIndividually = false;

	virtual double getTankFiringRateMultiplier() override { return .5; }

	MultishotTankPower();
};
