#pragma once
class HomingBulletPower;

#include "bulletpower.h"
#include "homingpower.h"

class HomingBulletPower : public BulletPower {
protected:
public:
	static const double homingStrength;

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return HomingPower::getClassColor();
	}

	virtual TankPower* makeTankPower();

	//bool modifiesMovement = false;
	virtual void modifiedMovement(Bullet*);
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithEdge = true;
	virtual PowerInteractionBoolHolder modifiedEdgeCollision(Bullet*);
	//bool overridesEdgeCollision = true;
	//bool modifiedEdgeCollisionCanWorkWithOthers = false;
	//bool modifiedEdgeCollisionCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() { return .5; }

	HomingBulletPower();
};
