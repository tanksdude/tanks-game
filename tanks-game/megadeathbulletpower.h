#pragma once
class MegaDeathBulletPower;

#include "power.h"
#include "bulletpower.h"
#include "megadeathpower.h"

class MegaDeathBulletPower : public BulletPower {
public:
	static const double destroyWallTier; //(and destroyHazardTier)
	static const double bulletSizeMultiplierPerTick;

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual void tick(Bullet* b); //for updating wall collision bools based on size

	virtual ColorValueHolder getColor() {
		return MegaDeathPower::getClassColor();
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

	//bullet-hazard collision now uses priority, making the below modification functions obsolete

	//bool modifiesCollisionWithCircleHazard = true;
	//virtual PowerInteractionBoolHolder modifiedCollisionWithCircleHazard(Bullet*, CircleHazard*);
	//bool overridesCollisionWithCircleHazard = true;
	//bool modifiedCollisionWithCircleHazardCanWorkWithOthers = true;
	//bool modifiedCollisionWithCircleHazardCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithRectHazard = true;
	//virtual PowerInteractionBoolHolder modifiedCollisionWithRectHazard(Bullet*, RectHazard*);
	//bool overridesCollisionWithRectHazard = true;
	//bool modifiedCollisionWithRectHazardCanWorkWithOthers = true;
	//bool modifiedCollisionWithRectHazardCanOnlyWorkIndividually = false;

	virtual double getOffenseImportance() { return 0; }
	virtual double getOffenseTier(Bullet*);
	virtual double getDefenseImportance() { return 0; }
	virtual double getDefenseTier(Bullet*);

	MegaDeathBulletPower();
};