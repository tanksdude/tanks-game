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
	double timeLeft;
	double maxTime; //set to -1 to last forever (which is normal for bullet powers)

public:
	virtual void initialize(Bullet* parent) { return; } //unlikely to be used
	virtual void removeEffects(Bullet* parent) { return; } //not really needed

	virtual void tick(Bullet*) { return; } //most will be doing a lot, though they shouldn't need this, but just in case
	void powerTick() {
		timeLeft--;
		//should not be virtual
	}
	bool isDone() const {
		/*
		if (maxTime < 0) [[likely]] {
			return false;
		}
		return (timeLeft <= 0);
		*/
		return ((maxTime >= 0) && (timeLeft <= 0));
	}
	virtual ColorValueHolder getColor() const = 0;
	virtual float getColorImportance() const { return 0; }
	virtual std::string getColorIdentifier() const = 0; //for caching colors

	virtual BulletPower* makeDuplicate() const = 0;
	virtual TankPower* makeTankPower() const = 0;

	bool modifiesMovement = false; //true if it, you know, modifies the movement
	//precondition: nothing
	virtual InteractionBoolHolder modifiedMovement(Bullet*) { return { false }; } //default does nothing, obviously
	bool overridesMovement = false; //true if the power completely changes how it moves; regular powers slightly modify movement (think homing) and still want basic bullet move
	bool modifiedMovementCanWorkWithOthers = true; //false stops later powerups in list from activating

	bool modifiesEdgeCollision = false;
	//precondition: was out of bounds, is not necessarily out of bounds
	virtual InteractionBoolHolder modifiedEdgeCollision(Bullet*) { return { false }; } //only the first false means something
	bool overridesEdgeCollision = true;
	bool modifiedEdgeCollisionCanWorkWithOthers = false; //options: either it bounces or temporarily stays outside, so it has no need to work with others; that makes the promise of powerup mixing kinda depressing

	bool modifiesCollisionWithTank = false;
	//precondition: hit tank, is not necessarily inside tank
	virtual InteractionBoolHolder modifiedCollisionWithTank(Bullet*, Tank*) { return { false, false }; }
	bool overridesCollisionWithTank = true;
	bool modifiedCollisionWithTankCanWorkWithOthers = true;

	bool modifiesCollisionWithWall = false;
	//precondition: hit wall, is not necessarily inside wall
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) { return { false, false, {}, {} }; }
	bool overridesCollisionWithWall = true; //false means also use the default, which is just destroy the bullet if it collides
	bool modifiedCollisionWithWallCanWorkWithOthers = true;

	//bool modifiesCollisionWithPowerup = false;
	//virtual void modifiedCollisionWithPowerSquare(Bullet*, PowerSquare*) { return; } //shouldn't be used

	//bool modifiesCollisionWithBullet = false;
	//virtual void modifiedCollisionWithBullet(Bullet* parent, Bullet* other) { return; } //probably shouldn't be used

	virtual bool getModifiesCollisionWithCircleHazard(const CircleHazard*) const { return false; }
	//precondition: hit circlehazard, is not necessarily inside circlehazard
	virtual InteractionBoolHolder modifiedCollisionWithCircleHazard(Bullet*, CircleHazard*) { return { false, false }; }
	bool overridesCollisionWithCircleHazard = true; //false means also use the default, which means destroy the bullet if it collides
	bool modifiedCollisionWithCircleHazardCanWorkWithOthers = true;

	virtual bool getModifiesCollisionWithRectHazard(const RectHazard*) const { return false; }
	//precondition: hit recthazard, is not necessarily inside recthazard
	virtual InteractionBoolHolder modifiedCollisionWithRectHazard(Bullet*, RectHazard*) { return { false, false }; }
	bool overridesCollisionWithRectHazard = true; //false means also use the default, which means destroy the bullet if it collides
	bool modifiedCollisionWithRectHazardCanWorkWithOthers = true;

	bool modifiesDeathHandling = false;
	virtual InteractionBoolHolder modifiedDeathHandling(Bullet* parent) { return { true, false }; } //first is bullet, second is bulletpower
	//it's first come, first served

	virtual double getBulletSpeedMultiplier() const { return 1; }
	bool bulletSpeedStacks = false;
	virtual double getBulletRadiusMultiplier() const { return 1; }
	bool bulletRadiusStacks = false;
	virtual double getBulletAcceleration() const { return 0; }
	virtual float getBulletAccelerationImportance() const { return 0; }
	virtual double getBulletDegradeAmount() const { return 0; } //negative values are not valid
	virtual float getBulletDegradeImportance() const { return 0; }
	virtual double getBulletRadiusGrowNumber_Stationary() const { return 0; }
	bool bulletRadiusGrowMultiplies_Stationary = false; //negative additive values do not get processed by bullets
	virtual double getBulletRadiusGrowNumber_Moving() const { return 0; }
	bool bulletRadiusGrowMultiplies_Moving = false; //same

	virtual float getOffenseImportance() const { return 0; } //"importance" = "override" value (when dealing with other powers)
	virtual float getOffenseTier(const Bullet*) const { return 0; }
	virtual float getDefenseImportance() const { return 0; }
	virtual float getDefenseTier(const Bullet*) const { return 0; }
	//idea: bulletpowers can set a flag for "additive offense/defense for same bulletpower"; allows double big bullets to be even stronger

	virtual ~BulletPower() { return; }
};
