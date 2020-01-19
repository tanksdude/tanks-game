#pragma once
class BigNamedBulletPower;

#include "power.h"
#include "bulletpower.h"
#include "bignamedpower.h"

class BigNamedBulletPower : public BulletPower {
protected:

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual void tick() { return; }
	virtual ColorValueHolder getColor() {
		return BigNamedPower::getClassColor();
	}

	virtual TankPower* makeTankPower();

	//bool modifiesCollisionWithWall = true;
	//virtual bool modifiedCollisionWithWall(Bullet*, Wall*);
	virtual bool modifiedCollisionWithWall(Bullet*, Wall*, int);
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getOffenseImportance() { return 0; }
	virtual double getOffenseTier() { return 2; }
	virtual double getDefenseImportance() { return 0; }
	virtual double getDefenseTier() { return 2; }

	BigNamedBulletPower();
};