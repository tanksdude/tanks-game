#pragma once
class BulletPower;

#include "interaction-bool-holder.h"
#include "color-value-holder.h"

#include "tank.h"
#include "bullet.h"
#include "wall.h"
#include "circle-hazard.h"
#include "rect-hazard.h"
#include "tank-power.h"

class BulletPower {
public:
	double timeLeft = 0;
	double maxTime = -1; //bullet powers typically last forever; setting this to -1 treats it as lasting forever

public:
	virtual void initialize(Bullet* parent) = 0; //unlikely to be used
	virtual void removeEffects(Bullet* parent) = 0; //not really needed

	virtual void tick(Bullet*) { return; } //most will be doing a lot, though they shouldn't need this, but just in case
	void powerTick() {
		timeLeft--;
		//should not be virtual
	}
	bool isDone() const {
		/*
		if (maxTime < 0) { [[likely]]
			return false;
		}
		return (timeLeft <= 0);
		*/
		return ((maxTime >= 0) && (timeLeft <= 0));
	}
	virtual ColorValueHolder getColor() const = 0;
	virtual float getColorImportance() const { return 0; }

	virtual BulletPower* makeDuplicate() const = 0;
	virtual TankPower* makeTankPower() const = 0;

	bool modifiesMovement = false; //true if it, you know, modifies the movement
	virtual InteractionBoolHolder modifiedMovement(Bullet*) { return { false }; } //default does nothing, obviously
	//precondition: nothing
	bool overridesMovement = false; //true if the power completely changes how it moves; regular powers slightly modify movement (think homing) and still want basic bullet move
	bool modifiedMovementCanWorkWithOthers = true; //false stops later powerups in list from activating
	bool modifiedMovementCanOnlyWorkIndividually = false; //true means that if another power was used previously, this power can't activate
	//TODO: have super override value? so the power can ensure that it and only it will activate (I don't think a power should have this kind of authority, but it might be needed)

	bool modifiesEdgeCollision = false;
	virtual InteractionBoolHolder modifiedEdgeCollision(Bullet*) { return { false }; } //only the first false means something
	//precondition: was out of bounds, is not necessarily out of bounds
	bool overridesEdgeCollision = true;
	bool modifiedEdgeCollisionCanWorkWithOthers = false; //options: either it bounces or temporarily stays outside, so it has no need to work with others; that makes the promise of powerup mixing kinda depressing
	bool modifiedEdgeCollisionCanOnlyWorkIndividually = false;

	bool modifiesCollisionWithTank = false;
	virtual InteractionBoolHolder modifiedCollisionWithTank(Bullet*, Tank*) { return { false, false }; }
	//precondition: hit tank, is not necessarily inside tank
	bool overridesCollisionWithTank = true;
	bool modifiedCollisionWithTankCanWorkWithOthers = true;
	bool modifiedCollisionWithTankCanOnlyWorkIndividually = false;

	bool modifiesCollisionWithWall = false;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) { return { false, false, {}, {} }; }
	//precondition: hit wall, is not necessarily inside wall
	bool overridesCollisionWithWall = true; //false means also use the default, which is just destroy the bullet if it collides
	bool modifiedCollisionWithWallCanWorkWithOthers = true;
	bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithPower = false;
	//virtual void modifiedCollisionWithPowerSquare(Bullet*, PowerSquare*) { return; } //probably shouldn't be used

	//bool modifiesCollisionWithBullet = false;
	//virtual void modifiedCollisionWithBullet(Bullet* parent, Bullet* other) { return; } //probably shouldn't be used

	virtual bool getModifiesCollisionWithCircleHazard(const CircleHazard*) const { return false; }
	virtual InteractionBoolHolder modifiedCollisionWithCircleHazard(Bullet*, CircleHazard*) { return { false, false }; }
	//precondition: hit circlehazard, is not necessarily inside circlehazard
	bool overridesCollisionWithCircleHazard = true; //false means also use the default, which means destroy the bullet if it collides
	bool modifiedCollisionWithCircleHazardCanWorkWithOthers = true;
	bool modifiedCollisionWithCircleHazardCanOnlyWorkIndividually = false;

	virtual bool getModifiesCollisionWithRectHazard(const RectHazard*) const { return false; }
	virtual InteractionBoolHolder modifiedCollisionWithRectHazard(Bullet*, RectHazard*) { return { false, false }; }
	//precondition: hit recthazard, is not necessarily inside recthazard
	bool overridesCollisionWithRectHazard = true; //false means also use the default, which means destroy the bullet if it collides
	bool modifiedCollisionWithRectHazardCanWorkWithOthers = true;
	bool modifiedCollisionWithRectHazardCanOnlyWorkIndividually = false;

	bool modifiesDeathHandling = false;
	virtual InteractionBoolHolder modifiedDeathHandling(Bullet* parent) { return { true, false }; } //first is bullet, second is bulletpower
	//it's first come, first served

	/*
	bool modifiesBulletDrawings = false;
	virtual void modifiedBulletDrawings(Bullet* parent) { return; } //probably not going to be used
	bool overridesBulletDrawings = false;
	bool modifiedBulletDrawingsCanWorkWithOthers = true;
	bool modifiedBulletDrawingsCanOnlyWorkIndividually = false;
	*/

	virtual double getBulletSpeedMultiplier() const { return 1; }
	bool bulletSpeedStacks = false;
	virtual double getBulletRadiusMultiplier() const { return 1; }
	bool bulletRadiusStacks = false;
	virtual double getBulletAcceleration() const { return 0; }
	virtual float getBulletAccelerationImportance() const { return 0; } //I don't think other functions should get importance values

	virtual float getOffenseImportance() const { return 0; } //"importance" = "override" value (when dealing with other powers)
	virtual float getOffenseTier(const Bullet*) const { return 0; }
	virtual float getDefenseImportance() const { return 0; }
	virtual float getDefenseTier(const Bullet*) const { return 0; }

	virtual ~BulletPower() { return; }
	//need separate offense stuff for different situations: tank vs bullet offense could be different (would it be more effective to have that stuff in modifiedTankCollision?)
};