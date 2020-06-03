#pragma once
class TripleTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
#include "triplepower.h"
#include "cannonpoint.h"

class TripleTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return TriplePower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	//bool modifiesAdditionalShooting = true;
	virtual void additionalShooting(Tank* parent, CannonPoint);
	//bool overridesAdditionalShooting = false;
	//bool additionalShootingCanWorkWithOthers = true;
	//bool additionalShootingCanOnlyWorkIndividually = false;

	TripleTankPower();
};
