#pragma once
class HomingBulletPower;

#include "bulletpower.h"
#include "homingpower.h"

class HomingBulletPower : public BulletPower {
protected: //public?
	static const double homingStrength;

public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() {
		return HomingPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new HomingBulletPower(); }
	virtual TankPower* makeTankPower();

	//bool modifiesMovement = false;
	virtual PowerInteractionBoolHolder modifiedMovement(Bullet*) override;
	//bool overridesMovement = false;
	//bool modifiedMovementCanWorkWithOthers = true;
	//bool modifiedMovementCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithEdge = true;
	virtual PowerInteractionBoolHolder modifiedEdgeCollision(Bullet*) override;
	//bool overridesEdgeCollision = true;
	//bool modifiedEdgeCollisionCanWorkWithOthers = false;
	//bool modifiedEdgeCollisionCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() override { return .5; }

	HomingBulletPower();
};
