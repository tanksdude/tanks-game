#pragma once
class GrenadeBulletPower;

#include "bulletpower.h"
#include "grenadepower.h"

class GrenadeBulletPower : public BulletPower {
protected:
	static const double degradeAmount;

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return GrenadePower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new GrenadeBulletPower(); }
	virtual TankPower* makeTankPower();

	//bool modifiesMovement = true;
	virtual PowerInteractionBoolHolder modifiedMovement(Bullet*);
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithWall = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*);
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getBulletAcceleration() { return -1.0/16; }

	GrenadeBulletPower();
};
