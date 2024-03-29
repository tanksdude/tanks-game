#pragma once
class TankPower;

#include <utility>
#include <vector>

#include "interaction-bool-holder.h"
#include "color-value-holder.h"

#include "tank.h"
#include "cannon-point.h"
#include "bullet.h"
#include "wall.h"
#include "circle-hazard.h"
#include "rect-hazard.h"
#include "bullet-power.h"

class TankPower {
public:
	double timeLeft;
	double maxTime;

public:
	virtual void initialize(Tank* parent) { return; }
	virtual void removeEffects(Tank* parent) { return; }

	virtual void tick(Tank*) { return; } //most shouldn't be doing anything
	void powerTick() {
		timeLeft--;
		//should not be virtual
	}
	bool isDone() const {
		/*
		if (maxTime < 0) [[unlikely]] {
			return false;
		}
		return (timeLeft <= 0);
		*/
		return ((maxTime >= 0) && (timeLeft <= 0));
	}
	virtual ColorValueHolder getColor() const = 0;
	virtual float getColorImportance() const { return 0; }

	virtual TankPower* makeDuplicate() const = 0;
	virtual BulletPower* makeBulletPower() const = 0;

	bool modifiesMovement = false;
	//precondition: nothing
	virtual InteractionBoolHolder modifiedMovement(Tank*, bool forward, bool turnL, bool turnR, bool specialKey) { return { false }; }
	bool overridesMovement = false; //set to true if the power completely changes how it moves; regular powers slightly modify movement and still want basic tank move
	bool modifiedMovementCanWorkWithOthers = true; //stops later powerups in list from activating

	bool modifiesEdgeCollision = false;
	//precondition: was out of bounds, is not necessarily out of bounds
	virtual InteractionBoolHolder modifiedEdgeCollision(Tank*) { return { false }; } //only the first false means something
	bool overridesEdgeCollision = true;
	bool modifiedEdgeCollisionCanWorkWithOthers = true;

	bool modifiesCollisionWithTank = false;
	//precondition: hit tank, is not necessarily inside tank
	virtual InteractionBoolHolder modifiedCollisionWithTank(Tank* parent, Tank* other) { return { false, false }; }
	bool overridesCollisionWithTank = true;
	bool modifiedCollisionWithTankCanWorkWithOthers = true;

	bool modifiesCollisionWithWall = false;
	//precondition: hit wall, is not necessariliy inside wall
	virtual InteractionBoolHolder modifiedCollisionWithWall(Tank*, Wall*) { return { false, false }; }
	bool overridesCollisionWithWall = true;
	bool modifiedCollisionWithWallCanWorkWithOthers = true;

	//bool modifiesCollisionWithPowerup = false;
	//virtual void modifiedCollisionWithPowerSquare(Tank*, PowerSquare*) { return; } //probably not going to be used

	//bool modifiesCollisionWithBullet = false;
	//virtual void modifiedCollisionWithBullet(Tank*, Bullet*) { return; } //probably shouldn't be used

	virtual bool getModifiesCollisionWithCircleHazard(const CircleHazard*) const { return false; }
	//precondition: hit circlehazard, is not necessarily inside circlehazard
	virtual InteractionBoolHolder modifiedCollisionWithCircleHazard(Tank*, CircleHazard*) { return { false, false }; }
	bool overridesCollisionWithCircleHazard = true; //false means also use the default, which means destroy the tank if it collides
	bool modifiedCollisionWithCircleHazardCanWorkWithOthers = true;

	virtual bool getModifiesCollisionWithRectHazard(const RectHazard*) const { return false; }
	//precondition: hit recthazard, is not necessarily inside recthazard
	virtual InteractionBoolHolder modifiedCollisionWithRectHazard(Tank*, RectHazard*) { return { false, false }; }
	bool overridesCollisionWithRectHazard = true; //false means also use the default, which means destroy the tank if it collides
	bool modifiedCollisionWithRectHazardCanWorkWithOthers = true;

	bool modifiesDeathHandling = false;
	virtual InteractionBoolHolder modifiedDeathHandling(Tank* parent) { return { true, false }; } //first is tank, second is tankpower
	//it's first come, first served

	/*
	bool modifiesShooting = false;
	virtual void modifiedShooting(Tank* parent) { return; } //for melee-class powerups (sword)
	bool overridesShooting = true; //false? dunno
	bool modifiedShootingCanWorkWithOthers = true;
	*/

	bool modifiesAdditionalShooting = false;
	virtual void additionalShooting(Tank* parent, const CannonPoint&, const ExtraCannonPoint&) { return; } //for regular powerups (fire/blast, mines(?))
	bool overridesAdditionalShooting = false; //true if default shooting won't be used
	bool additionalShootingCanWorkWithOthers = true; //probably should only be true

	bool addsShootingPoints = false;
	[[nodiscard]]
	virtual std::vector<double>* addShootingPoints() const { return nullptr; } //list of angles to put new cannons at; angle range: (0,1)
	//bool overridesAddShootingPoints = false; //makes no sense to be true
	bool addShootingPointsCanWorkWithOthers = true; //should only be true

	bool addsExtraShootingPoints = false;
	[[nodiscard]]
	virtual std::vector<std::pair<double, double>>* addExtraShootingPoints() const { return nullptr; } //list of angles to put "extra" cannons at (triple and shotgun)
	//bool overridesAddExtraShootingPoints = false; //makes no sense to be true
	bool addExtraShootingPointsCanWorkWithOthers = true; //should only be true

	/*
	bool modifiesTankDrawings = false;
	//TODO: this can work; maybe pass in tank radius and just the first cannon
	virtual void modifiedTankDrawings(Tank* parent) { return; }
	bool overridesTankDrawings = false;
	bool modifiedTankDrawingsCanWorkWithOthers = true;
	*/

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

	virtual float getOffenseImportance() const { return 0; } //"importance" = "override" value (when dealing with other powers)
	virtual float getOffenseTier(const Tank*) const { return 0; }
	virtual float getDefenseImportance() const { return 0; }
	virtual float getDefenseTier(const Tank*) const { return 0; }

	virtual ~TankPower() { return; }
};
