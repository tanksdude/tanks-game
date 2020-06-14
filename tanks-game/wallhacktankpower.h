#pragma once
class WallhackTankPower;

#include "tankpower.h"
#include "wallhackpower.h"

class WallhackTankPower : public TankPower {

public:
	void initialize(Tank* parent);
	void removeEffects(Tank* parent);

	virtual ColorValueHolder getColor() {
		return WallhackPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new WallhackTankPower(); }
	virtual BulletPower* makeBulletPower();

	//bool modifiesCollisionWithWall = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Tank*, Wall*);
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	WallhackTankPower();
};
