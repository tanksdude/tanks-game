#pragma once
class TankPower;

#include "colorvalueholder.h"
#include "tank.h"
#include "cannonpoint.h"
#include "bullet.h"
#include "wall.h"
#include "inheritedpowercommon.h"

class TankPower : public InheritedPowerCommon {
protected:
	//double timeLeft;
	//double maxTime;

public:
	virtual void initialize(Tank* parent) = 0;
	virtual void removeEffects(Tank* parent) = 0;

	virtual void tick() {
		//most shouldn't be doing anything
		return;
	}
	virtual void powerTick() {
		timeLeft--;
	}
	virtual void powerTick(Tank*) { powerTick(); }
	virtual bool isDone() {
		return timeLeft <= 0;
	}
	virtual ColorValueHolder getColor() = 0;

	virtual BulletPower* makeBulletPower() = 0;

	bool modifiesMovement = false;
	virtual void modifiedMovement(Tank*) { return; }
	//precondition: nothing
	bool overridesMovement = false; //set to true if the power completely changes how it moves; regular powers slightly modify movement and still want basic tank move
	bool modifiedMovementCanWorkWithOthers = true; //stops later powerups in list from activating
	bool modifiedMovementCanOnlyWorkIndividually = false; //if another power was used previously, this power can't activate

	bool modifiesCollisionWithEdge = false;
	virtual PowerInteractionBoolHolder modifiedEdgeCollision(Tank*) { return { false, false }; }
	//precondition: was out-of-bounds, is not necessarily out-of-bounds
	bool overridesEdgeCollision = true;
	bool modifiedEdgeCollisionCanWorkWithOthers = true;
	bool modifiedEdgeCollisionCanOnlyWorkIndividually = false;

	bool modifiesCollisionWithTank = false;
	virtual PowerInteractionBoolHolder modifiedCollisionWithTank(Tank* parent, Tank* other) { return { false, false }; }
	//precondition: hit tank, is not necessarily inside tank
	bool overridesCollisionWithTank = true;
	bool modifiedCollisionWithTankCanWorkWithOthers = true;
	bool modifiedCollisionWithTankCanOnlyWorkIndividually = false;
	
	bool modifiesCollisionWithWall = false;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Tank*, Wall*) { return { false, false }; }
	//precondition: hit wall, is not necessariliy inside wall
	bool overridesCollisionWithWall = true;
	bool modifiedCollisionWithWallCanWorkWithOthers = true;
	bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithPower = false;
	//virtual void modifiedCollisionWithPower(Tank*, Power*) { return; } //probably not going to be used

	//bool modifiesCollisionWithBullet = false;
	//virtual void modifiedCollisionWithBullet(Tank*, Bullet*) { return; } //probably shouldn't be used

	//bool modifiesCollisionWithHazard = false;
	//virtual void modifiedCollisionWithHazard(Tank*, Hazard*) { return; }

	bool modifiesShooting = false;
	virtual void modifiedShooting(Tank* parent) { return; } //for melee-class powerups
	bool overridesShooting = true; //false? dunno
	bool modifiedShootingCanWorkWithOthers = true;
	bool modifiedShootingCanOnlyWorkIndividually = false;

	//TODO: modify this so something like fire + multishot + triple will work beter (not sure that would be the case, actually)
	bool modifiesAdditionalShooting = false;
	virtual void additionalShooting(Tank* parent, CannonPoint) { return; } //for regular powerups (triple and shotgun)
	bool overridesAdditionalShooting = false; //probably should only be false
	bool additionalShootingCanWorkWithOthers = true; //probably should only be true
	bool additionalShootingCanOnlyWorkIndividually = false;

	bool addsShootingPoints = false;
	virtual void addShootingPoints(Tank*, std::vector<CannonPoint>*) { return; } //shouldn't need the Tank*
	bool overridesAddShootingPoints = false; //should only be false
	bool addShootingPointsCanWorkWithOthers = true; //should only be true
	bool addShootingPointsCanOnlyWorkIndividually = false;
	
	bool modifiesTankDrawings = false;
	virtual void modifiedTankDrawings(Tank* parent) { return; }
	bool overridesTankDrawings = false;
	bool modifiedTankDrawingsCanWorkWithOthers = true;
	bool modifiedTankDrawingsCanOnlyWorkIndividually = false;

	virtual double getShootingMultiplier() { return 1; }
	virtual double getBulletSpeedMultiplier() { return 1; }
	virtual double getBulletRadiusMultiplier() { return 1; }
	virtual double getBulletAcceleration() { return 0; }

	virtual double getOffenseImportance() { return 0; } //"importance" = "override" value (when dealing with other powers)
	virtual double getOffenseTier(Tank*) { return 0; }
	virtual double getDefenseImportance() { return 0; }
	virtual double getDefenseTier(Tank*) { return 0; }

};
