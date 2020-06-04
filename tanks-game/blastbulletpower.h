#pragma once
class BlastBulletPower;

#include "bulletpower.h"
#include "blastpower.h"

class BlastBulletPower : public BulletPower {
protected:
	static const double degradeAmount;

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual ColorValueHolder getColor() {
		return BlastPower::getClassColor();
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

	BlastBulletPower();
};
