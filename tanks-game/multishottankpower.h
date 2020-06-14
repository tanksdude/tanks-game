#pragma once
class MultishotTankPower;

#include "tankpower.h"
#include "multishotpower.h"

class MultishotTankPower : public TankPower {
public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

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

	virtual double getFiringRateMultiplier() { return .5; }

	MultishotTankPower();
};
