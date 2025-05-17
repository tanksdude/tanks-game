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
	virtual void initialize(Bullet* parent) { return; } //unlikely to be used
	virtual void removeEffects(Bullet* parent) { return; } //not really needed

	virtual void tick(Bullet*) { return; } //most will be doing a lot, though they shouldn't need this, but just in case
	virtual ColorValueHolder getColor() const = 0;
	virtual float getColorImportance() const { return 0; }
	virtual std::string getColorIdentifier() const = 0; //for caching colors

	virtual BulletPower* makeDuplicate() const = 0;
	virtual TankPower* makeTankPower() const = 0;

	bool modifiesMovement = false;
	virtual InteractionBoolHolder modifiedMovement(Bullet*) { return { false }; } //default does nothing, obviously
	bool overridesMovement = false; //true if the power completely changes how it moves; regular powers slightly modify movement (think homing) and still want basic bullet move
	bool modifiedMovementCanWorkWithOthers = true; //false stops later powerups in list from activating

	bool modifiesEdgeCollision = false;
	//precondition: was out of bounds, is not necessarily out of bounds anymore
	virtual InteractionBoolHolder modifiedEdgeCollision(Bullet*) { return { false }; } //only the first false means something
	bool overridesEdgeCollision = true;
	bool modifiedEdgeCollisionCanWorkWithOthers = false; //options: either it bounces or temporarily stays outside, so it has no need to work with others; that makes the promise of powerup mixing kinda depressing

	bool modifiesCollisionWithTank = false;
	virtual InteractionUpdateHolder<BulletUpdateStruct, TankUpdateStruct> modifiedCollisionWithTank(const Bullet*, const Tank*) { return { false, false, {}, {} }; }
	bool overridesCollisionWithTank = true;
	bool modifiedCollisionWithTankCanWorkWithOthers = true;

	//bool modifiesCollisionWithBullet = false;
	//virtual InteractionUpdateHolder<BulletUpdateStruct, BulletUpdateStruct> modifiedCollisionWithBullet(Bullet* parent, Bullet* other) { return; } //huge potential for performance issues, so don't use

	bool modifiesCollisionWithWall = false;
	virtual InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> modifiedCollisionWithWall(const Bullet*, const Wall*) { return { false, false, {}, {} }; }
	bool overridesCollisionWithWall = true; //false means also use the default, which is just destroy the bullet if it collides
	bool modifiedCollisionWithWallCanWorkWithOthers = true;

	virtual bool getModifiesCollisionWithCircleHazard(const CircleHazard*) const { return false; }
	virtual InteractionUpdateHolder<BulletUpdateStruct, CircleHazardUpdateStruct> modifiedCollisionWithCircleHazard(const Bullet*, const CircleHazard*) { return { false, false, {}, {} }; }
	bool overridesCollisionWithCircleHazard = true; //false means also use the default, which means destroy the bullet if it collides
	bool modifiedCollisionWithCircleHazardCanWorkWithOthers = true;

	virtual bool getModifiesCollisionWithRectHazard(const RectHazard*) const { return false; }
	virtual InteractionUpdateHolder<BulletUpdateStruct, RectHazardUpdateStruct> modifiedCollisionWithRectHazard(const Bullet*, const RectHazard*) { return { false, false, {}, {} }; }
	bool overridesCollisionWithRectHazard = true; //false means also use the default, which means destroy the bullet if it collides
	bool modifiedCollisionWithRectHazardCanWorkWithOthers = true;

	bool modifiesDeathHandling = false;
	virtual InteractionBoolHolder modifiedDeathHandling(const Bullet* parent) { return { true, false }; } //first is bullet, second is bulletpower
	//it's first come, first served

	virtual float getBulletSpeedMultiplier() const { return 1; }
	bool bulletSpeedStacks = false;
	virtual double getBulletRadiusMultiplier() const { return 1; }
	bool bulletRadiusStacks = false;
	virtual float getBulletAcceleration() const { return 0; }
	virtual float getBulletAccelerationImportance() const { return 0; }
	virtual float getBulletDegradeAmount() const { return 0; } //negative values are not valid
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
