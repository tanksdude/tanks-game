#pragma once
class BulletPower;
//beginning to realize how much a template would help

#include "colorvalueholder.h"
#include "tank.h"
#include "bullet.h"
#include "wall.h"
#include "circlehazard.h"
#include "recthazard.h"
#include "inheritedpowercommon.h"
#include "tankpower.h"

//TODO long-term: look into std::shared_ptr<BulletPower> for stronger memory management
//(note to self: a bulletpower would need to make a "child" of itself in the case of banana (delete a banana from list of bulletpowers), in order to have super strong memory management)
//that would probably be really complex to fully implement (but simple enough to think about? maybe it is simple to implement) so that will wait

class BulletPower : public InheritedPowerCommon{
	friend class PowerFunctionHelper;
protected:
public:
	//double timeLeft = 0;
	//double maxTime = -1; //bullet powers typically last forever; setting this to -1 treats it as lasting forever

public:
	virtual void initialize(Bullet* parent) = 0; //unlikely to be used
	virtual void removeEffects(Bullet* parent) = 0; //not really needed

	virtual void tick(Bullet*) { return; } //most will be doing a lot, though they shouldn't need this, but just in case
	virtual void powerTick(Bullet*) {
		timeLeft--; //not like any bulletpower is limited, though
	}
	virtual bool isDone() { //typically, this will always be false
		if (maxTime < 0) {
			return false;
		}
		return (timeLeft <= 0);
	}
	virtual ColorValueHolder getColor() = 0;

	virtual TankPower* makeTankPower() = 0;

	bool modifiesMovement = false; //true if it, you know, modifies the movement
	virtual void modifiedMovement(Bullet*) { return; } //default does nothing, obviously
	//precondition: nothing
	bool overridesMovement = false; //true if the power completely changes how it moves; regular powers slightly modify movement (think homing) and still want basic bullet move
	bool modifiedMovementCanWorkWithOthers = true; //false stops later powerups in list from activating
	bool modifiedMovementCanOnlyWorkIndividually = false; //true means that if another power was used previously, this power can't activate
	//fix: have super override value? so the power can ensure that it and only it will activate (I don't think a power should have this kind of authority, but it might be needed)

	bool modifiesEdgeCollision = false;
	virtual PowerInteractionBoolHolder modifiedEdgeCollision(Bullet*) { return { false, false }; }
	//precondition: was out-of-bounds, is not necessarily out-of-bounds
	bool overridesEdgeCollision = true;
	bool modifiedEdgeCollisionCanWorkWithOthers = false; //options: either it bounces or temporarily stays outside, so it has no need to work with others; that makes the promise of powerup mixing kinda depressing
	bool modifiedEdgeCollisionCanOnlyWorkIndividually = false;

	bool modifiesCollisionWithTank = false;
	virtual PowerInteractionBoolHolder modifiedCollisionWithTank(Bullet*, Tank*) { return { false, false }; }
	//precondition: hit tank, is not necessarily inside tank
	bool overridesCollisionWithTank = true;
	bool modifiedCollisionWithTankCanWorkWithOthers = true;
	bool modifiedCollisionWithTankCanOnlyWorkIndividually = false;

	bool modifiesCollisionWithWall = false;
	virtual PowerInteractionBoolHolder modifiedCollisionWithWall(Bullet*, Wall*) { return { false, false }; }
	//precondition: hit wall, is not necessarily inside wall
	bool overridesCollisionWithWall = true; //false means also use the default, which is just destroy the bullet if it collides
	bool modifiedCollisionWithWallCanWorkWithOthers = true;
	bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithPower = false;
	//virtual void modifiedCollisionWithPower(Bullet*, Power*) { return; } //probably shouldn't be used

	//bool modifiesCollisionWithBullet = false;
	//virtual void modifiedCollisionWithBullet(Bullet* parent, Bullet* other) { return; } //probably shouldn't be used

	bool modifiesCollisionWithCircleHazard = false;
	virtual PowerInteractionBoolHolder modifiedCollisionWithCircleHazard(Bullet*, CircleHazard*) { return { false, false }; }
	//precondition: hit circlehazard, is not necessarily inside circlehazard
	bool overridesCollisionWithCircleHazard = true; //false means also use the default, which means destroy the bullet if it collides
	bool modifiedCollisionWithCircleHazardCanWorkWithOthers = true;
	bool modifiedCollisionWithCircleHazardCanOnlyWorkIndividually = false;

	bool modifiesCollisionWithRectHazard = false;
	virtual PowerInteractionBoolHolder modifiedCollisionWithRectHazard(Bullet*, RectHazard*) { return { false, false }; }
	//precondition: hit recthazard, is not necessarily inside recthazard
	bool overridesCollisionWithRectHazard = true; //false means also use the default, which means destroy the bullet if it collides
	bool modifiedCollisionWithRectHazardCanWorkWithOthers = true;
	bool modifiedCollisionWithRectHazardCanOnlyWorkIndividually = false;

	bool modifiesBulletDrawings = false;
	virtual void modifiedBulletDrawings(Bullet* parent) { return; } //probably not going to be used
	bool overridesBulletDrawings = false;
	bool modifiedBulletDrawingsCanWorkWithOthers = true;
	bool modifiedBulletDrawingsCanOnlyWorkIndividually = false;

	virtual double getOffenseImportance() { return 0; } //"importance" = "override" value (when dealing with other powers)
	virtual double getOffenseTier(Bullet*) { return 0; }
	virtual double getDefenseImportance() { return 0; }
	virtual double getDefenseTier(Bullet*) { return 0; }

	//need separate offense stuff for different situations: tank vs bullet offense could be different (would it be more effective to have that stuff in modifiedTankCollision?)
};
