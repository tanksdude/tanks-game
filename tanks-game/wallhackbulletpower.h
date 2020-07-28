#pragma once
class WallhackBulletPower;

#include "bulletpower.h"
#include "wallhackpower.h"

class WallhackBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() const override {
		return WallhackPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() const override { return new WallhackBulletPower(); }
	virtual TankPower* makeTankPower() const override;
	
	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) override;
	//bool overridesCollisionWithWall = true; //false means also use the default
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	WallhackBulletPower();
};
