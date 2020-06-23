#pragma once
class WallhackBulletPower;

#include "bulletpower.h"
#include "wallhackpower.h"

class WallhackBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() {
		return WallhackPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new WallhackBulletPower(); }
	virtual TankPower* makeTankPower();
	
	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) override;
	//bool overridesCollisionWithWall = true; //false means also use the default
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	WallhackBulletPower();
};
