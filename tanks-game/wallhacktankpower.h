#pragma once
class WallhackTankPower;

#include "tankpower.h"
#include "wallhackpower.h"

class WallhackTankPower : public TankPower {
public:
	virtual void initialize(Tank* parent) override;
	virtual void removeEffects(Tank* parent) override;

	virtual ColorValueHolder getColor() const override {
		return WallhackPower::getClassColor();
	}

	virtual TankPower* makeDuplicate() const override { return new WallhackTankPower(); }
	virtual BulletPower* makeBulletPower() const override;

	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Tank*, Wall*) override;
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	WallhackTankPower();
};
