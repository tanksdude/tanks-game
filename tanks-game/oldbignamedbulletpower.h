#pragma once
class OldBigNamedBulletPower;

#include "bulletpower.h"
#include "oldbignamedpower.h"
#include "constants.h"

class OldBigNamedBulletPower : public BulletPower {
public:
	virtual void initialize(Bullet* parent) override;
	virtual void removeEffects(Bullet* parent) override;

	virtual ColorValueHolder getColor() {
		return OldBigNamedPower::getClassColor();
	}

	virtual BulletPower* makeDuplicate() { return new OldBigNamedBulletPower(); }
	virtual TankPower* makeTankPower();

	//bool modifiesCollisionWithWall = true;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) override;
	//bool overridesCollisionWithWall = true;
	//bool modifiedCollisionWithWallCanWorkWithOthers = true;
	//bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	virtual double getBulletSpeedMultiplier() override { return .5; }
	//bool bulletSpeedStacks = true;
	virtual double getBulletRadiusMultiplier() override { return 4; }
	//bool bulletRadiusStacks = true;

	virtual double getOffenseImportance() override { return 0; }
	virtual double getOffenseTier(Bullet*) override { return DESTRUCTION_TIER; }
	virtual double getDefenseImportance() override { return 0; }
	virtual double getDefenseTier(Bullet*) override { return DESTRUCTION_TIER; }

	OldBigNamedBulletPower();
};
