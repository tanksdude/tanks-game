#pragma once
class WallhackTankPower;

#include "power.h"
#include "tankpower.h"
#include "bulletpower.h"
#include "wallhackpower.h"

class WallhackTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return WallhackPower::getClassColor();
	}

	virtual BulletPower* makeBulletPower();

	//bool modifiesCollisionWithWall = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Tank*, Wall*);
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	WallhackTankPower();
};
