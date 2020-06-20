#pragma once
class MultishotTankPower;

#include "tankpower.h"
#include "multishotpower.h"

class MultishotTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent);
	virtual void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return MultishotPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new MultishotTankPower(); }
	virtual BulletPower* makeBulletPower();

	//bool addsShootingPoints = true;
	virtual void addShootingPoints(Tank*, std::vector<CannonPoint>*);
	//bool overridesAddShootingPoints = false;
	//bool addShootingPointsCanWorkWithOthers = true;
	//bool addShootingPointsCanOnlyWorkIndividually = false;

	virtual double getTankFiringRateMultiplier() { return .5; }

	MultishotTankPower();
};
