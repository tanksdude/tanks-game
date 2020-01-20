#pragma once
class MultishotTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
#include "multishotpower.h"
#include "cannonpoint.h"

class MultishotTankPower : public TankPower {
public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return MultishotPower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	//bool addsShootingPoints = true;
	virtual void addShootingPoints(Tank*, std::vector<CannonPoint>*);
	//bool overridesAddShootingPoints = false;
	//bool addShootingPointsCanWorkWithOthers = true;
	//bool addShootingPointsCanOnlyWorkIndividually = false;

	virtual double getShootingMultiplier() { return .5; }

	MultishotTankPower();
};