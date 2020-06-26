#pragma once
class BigNamedBulletPower;

#include "bulletpower.h"
#include "bignamedpower.h"
#include "constants.h"

class BigNamedBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() {
		return BigNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new BigNamedBulletPower(); }
	virtual TankPower* makeTankPower();

	//bool modifiesCollisionWithWall = true;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) override;
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() override { return .25; }
	virtual double getBulletRadiusMultiplier() override { return 4; }
	//bool bulletRadiusStacks = true;

	virtual double getOffenseImportance() override { return 0; }
	virtual double getOffenseTier(Bullet*) override { return DESTRUCTION_TIER; }
	virtual double getDefenseImportance() override { return 0; }
	virtual double getDefenseTier(Bullet*) override { return DESTRUCTION_TIER; }

	BigNamedBulletPower();
};
