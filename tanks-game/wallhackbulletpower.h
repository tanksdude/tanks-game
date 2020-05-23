#pragma once
class WallhackBulletPower;

#include "power.h"
#include "bulletpower.h"
#include "wallhackpower.h"

class WallhackBulletPower : public BulletPower {

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return WallhackPower::getClassColor();
	}

	virtual TankPower* makeTankPower();
	
	//bool modifiesCollisionWithWall = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*);
	//bool overridesCollisionWithWall = true; //false means also use the default
	//bool modifiedCollisionWithWallCanWorkWithOthers = false;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	WallhackBulletPower();
};