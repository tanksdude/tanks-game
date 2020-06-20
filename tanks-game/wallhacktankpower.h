#pragma once
class WallhackTankPower;

#include "tankpower.h"
#include "wallhackpower.h"

class WallhackTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() {
		return WallhackPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() { return new WallhackTankPower(); }
	virtual BulletPower* makeBulletPower();

	//bool modifiesCollisionWithWall = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Tank*, Wall*) override;
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	WallhackTankPower();
};
