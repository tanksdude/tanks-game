#pragma once
class TankPower;

#include "inheritedpowercommon.h"
#include "colorvalueholder.h"
#include "tank.h"
#include "cannonpoint.h"
#include "bullet.h"
#include "wall.h"
#include "circlehazard.h"
#include "recthazard.h"
#include "bulletpower.h"

class TankPower : public InheritedPowerCommon {
protected:
	//double timeLeft;
	//double maxTime;

public:
	virtual void initialize(Tank* parent) = 0;
	virtual void removeEffects(Tank* parent) = 0;

	virtual void tick(Tank*) {
		//most shouldn't be doing anything
		return;
	}
	virtual void powerTick(Tank*) {
		timeLeft--;
	}
	virtual bool isDone() const {
		if (maxTime < 0) {
			return false;
		}
		return (timeLeft <= 0);
	}
	virtual ColorValueHolder getColor() const = 0;
	virtual double getColorImportance() const { return 0; }

	virtual TankPower* makeDuplicate() const = 0;
	virtual BulletPower* makeBulletPower() const = 0;

	bool modifiesMovement = false;
	virtual InteractionBoolHolder modifiedMovement(Tank*) { return { false }; }
	//precondition: nothing
	bool overridesMovement = false; //set to true if the power completely changes how it moves; regular powers slightly modify movement and still want basic tank move
	bool modifiedMovementCanWorkWithOthers = true; //stops later powerups in list from activating
	bool modifiedMovementCanOnlyWorkIndividually = false; //if another power was used previously, this power can't activate

	bool modifiesEdgeCollision = false;
	virtual InteractionBoolHolder modifiedEdgeCollision(Tank*) { return { false }; } //only the first false means something
	//precondition: was out of bounds, is not necessarily out of bounds
	bool overridesEdgeCollision = true;
	bool modifiedEdgeCollisionCanWorkWithOthers = true;
	bool modifiedEdgeCollisionCanOnlyWorkIndividually = false;

	bool modifiesCollisionWithTank = false;
	virtual InteractionBoolHolder modifiedCollisionWithTank(Tank* parent, Tank* other) { return { false, false }; }
	//precondition: hit tank, is not necessarily inside tank
	bool overridesCollisionWithTank = true;
	bool modifiedCollisionWithTankCanWorkWithOthers = true;
	bool modifiedCollisionWithTankCanOnlyWorkIndividually = false;

	bool modifiesCollisionWithWall = false;
	virtual InteractionBoolHolder modifiedCollisionWithWall(Tank*, Wall*) { return { false, false }; }
	//precondition: hit wall, is not necessariliy inside wall
	bool overridesCollisionWithWall = true;
	bool modifiedCollisionWithWallCanWorkWithOthers = true;
	bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithPower = false;
	//virtual void modifiedCollisionWithPowerSquare(Tank*, PowerSquare*) { return; } //probably not going to be used

	//bool modifiesCollisionWithBullet = false;
	//virtual void modifiedCollisionWithBullet(Tank*, Bullet*) { return; } //probably shouldn't be used

	virtual bool getModifiesCollisionWithCircleHazard(const CircleHazard*) const { return false; }
	virtual InteractionBoolHolder modifiedCollisionWithCircleHazard(Tank*, CircleHazard*) { return { false, false }; }
	//precondition: hit circlehazard, is not necessarily inside circlehazard
	bool overridesCollisionWithCircleHazard = true; //false means also use the default, which means destroy the bullet if it collides
	bool modifiedCollisionWithCircleHazardCanWorkWithOthers = true;
	bool modifiedCollisionWithCircleHazardCanOnlyWorkIndividually = false;

	virtual bool getModifiesCollisionWithRectHazard(const RectHazard*) const { return false; }
	virtual InteractionBoolHolder modifiedCollisionWithRectHazard(Tank*, RectHazard*) { return { false, false }; }
	//precondition: hit recthazard, is not necessarily inside recthazard
	bool overridesCollisionWithRectHazard = true; //false means also use the default, which means destroy the bullet if it collides
	bool modifiedCollisionWithRectHazardCanWorkWithOthers = true;
	bool modifiedCollisionWithRectHazardCanOnlyWorkIndividually = false;

	bool modifiesDeathHandling = false;
	virtual InteractionBoolHolder modifiedDeathHandling(Tank* parent) { return { true, false }; } //first is tank, second is tankpower
	//it's first come, first served

	/*
	bool modifiesShooting = false;
	virtual void modifiedShooting(Tank* parent) { return; } //for melee-class powerups
	bool overridesShooting = true; //false? dunno
	bool modifiedShootingCanWorkWithOthers = true;
	bool modifiedShootingCanOnlyWorkIndividually = false;
	*/

	bool modifiesAdditionalShooting = false;
	virtual void additionalShooting(Tank* parent, CannonPoint) { return; } //for regular powerups (triple/shotgun, fire/blast, mines(?))
	bool overridesAdditionalShooting = false; //true if default shooting won't be used
	bool additionalShootingCanWorkWithOthers = true; //probably should only be true
	bool additionalShootingCanOnlyWorkIndividually = false;

	bool addsShootingPoints = false;
	virtual void addShootingPoints(Tank*, std::vector<CannonPoint>*) { return; } //shouldn't need the Tank*
	//bool overridesAddShootingPoints = false; //makes no sense to be true
	bool addShootingPointsCanWorkWithOthers = true; //should only be true
	bool addShootingPointsCanOnlyWorkIndividually = false;

	bool modifiesTankDrawings = false;
	virtual void modifiedTankDrawings(Tank* parent) { return; }
	bool overridesTankDrawings = false;
	bool modifiedTankDrawingsCanWorkWithOthers = true;
	bool modifiedTankDrawingsCanOnlyWorkIndividually = false;

	virtual double getTankMaxSpeedMultiplier() const { return 1; }
	bool tankMaxSpeedStacks = false;
	virtual double getTankAccelerationMultiplier() const { return 1; }
	bool tankAccelerationStacks = false;
	virtual double getTankRadiusMultiplier() const { return 1; }
	bool tankRadiusStacks = false;
	virtual double getTankFiringRateMultiplier() const { return 1; }
	bool tankFiringRateStacks = false;
	virtual double getTankTurningIncrementMultiplier() const { return 1; }
	bool tankTurningIncrementStacks = false;

	virtual double getOffenseImportance() const { return 0; } //"importance" = "override" value (when dealing with other powers)
	virtual double getOffenseTier(const Tank*) const { return 0; }
	virtual double getDefenseImportance() const { return 0; }
	virtual double getDefenseTier(const Tank*) const { return 0; }

	virtual ~TankPower() { return; }
};
