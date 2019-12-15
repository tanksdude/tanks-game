#pragma once
class BulletPower;
//beginning to realize how much a template would help

#include "colorvalueholder.h"
#include "tank.h"
#include "bullet.h"
#include "wall.h"
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
	//double maxTime = -1; //bullet powers typically last forever, so they should default to -1

public:
	virtual void initialize(Bullet* parent) = 0; //unlikely to be used
	virtual void removeEffects(Bullet* parent) = 0; //not really needed

	virtual void tick() = 0; //most will be doing a lot, though they shouldn't need this, but just in case
	virtual void powerTick() { return; }
	virtual bool isDone() { return false; }
	virtual ColorValueHolder getColor() = 0;

	virtual TankPower* makeTankPower() = 0;

	bool modifiesMovement = false;
	virtual void modifiedMovement(Bullet*) { return; }
	bool overridesMovement = false; //set to true if the power completely changes how it moves; regular powers slightly modify movement and still want basic bullet move
	bool modifiedMovementCanWorkWithOthers = true; //stops later powerups in list from activating
	bool modifiedMovementCanOnlyWorkIndividually = false; //if another power was used previously, this power can't activate
	//fix: have override value? so the power can ensure that it and only it will activate (I don't think a power should have this kind of authority, but it might be needed)

	bool modifiesCollisionWithEdge = false;
	virtual bool modifiedEdgeCollision(Bullet*) { return false; }
	bool overridesEdgeCollision = true;
	bool modifiedEdgeCollisionCanWorkWithOthers = false; //options: either it bounces or temporarily stays outside, so it has no need to work with others; does make the promise of powerup mixing kinda depressing
	bool modifiedEdgeCollisionCanOnlyWorkIndividually = false;

	bool modifiesCollisionWithTank = false;
	virtual bool modifiedCollisionWithTank(Bullet*, Tank*) { return false; }
	bool overridesCollisionWithTank = true;
	bool modifiedCollisionWithTankCanWorkWithOthers = true;
	bool modifiedCollisionWithTankCanOnlyWorkIndividually = false;

	bool modifiesCollisionWithWall = false;
	virtual bool modifiedCollisionWithWall(Bullet*, Wall*) { return false; }
	bool overridesCollisionWithWall = true; //false means also use the default
	bool modifiedCollisionWithWallCanWorkWithOthers = true;
	bool modifiedCollisionWithWallCanOnlyWorkIndividually = false;

	//bool modifiesCollisionWithPower = false;
	//virtual void modifiedCollisionWithPower(Bullet*, Power*) { return; } //probably shouldn't be used

	//bool modifiesCollisionWithBullet = false;
	//virtual void modifiedCollisionWithBullet(Bullet* parent, Bullet* other) { return; } //probably shouldn't be used

	//bool modifiesCollisionWithHazard = false;
	//virtual void modifiedCollisionWithHazard(Bullet*, Hazard*);

	bool modifiesBulletDrawings = false;
	virtual void modifiedBulletDrawings(Bullet* parent) { return; } //probably not going to be used
	bool overridesBulletDrawings = false;
	bool modifiedBulletDrawingsCanWorkWithOthers = true;
	bool modifiedBulletDrawingsCanOnlyWorkIndividually = false;

	//virtual double getOffenseTier() { return 0; }
	//virtual double getOffenseValue() { return 0; }
	//virtual double getDefenseTier() { return 0; }
	//virtual double getDefenseValue() { return 0; }
};