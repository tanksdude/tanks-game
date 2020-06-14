#pragma once
class BlastBulletPower;

#include "bulletpower.h"
#include "blastpower.h"

class BlastBulletPower : public BulletPower {
protected:
	static const double maxBulletAcceleration;
	static const double minBulletAcceleration;
	static const double degradeAmount;
	double accelerationAmount;

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return BlastPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate();
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

	virtual double getBulletRadiusMultiplier() { return .25; }
	virtual double getBulletAcceleration();

	BlastBulletPower();
	BlastBulletPower(double acceleration); //protected?
};
