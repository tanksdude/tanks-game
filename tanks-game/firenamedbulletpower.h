#pragma once
class FireNamedBulletPower;

#include "bulletpower.h"
#include "firenamedpower.h"

class FireNamedBulletPower : public BulletPower {
protected:
	static const double degradeAmount;
	static const double growAmount;

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return FireNamedPower::getClassColor();
	}

	virtual TankPower* makeTankPower();

	//bool modifiesMovement = true;
	virtual void modifiedMovement(Bullet*);
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithWall = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*);
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	//virtual double getBulletAcceleration() { return -1.0/16; }
	virtual double getBulletSpeedMultiplier() { return .5; }

	FireNamedBulletPower();
};
