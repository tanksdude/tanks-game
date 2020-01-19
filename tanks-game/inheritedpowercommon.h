#pragma once
class InheritedPowerCommon;

#include "colorvalueholder.h"
#include "tank.h"
#include "wall.h"
//#include "power.h"
//#include "bullet.h"

//TODO: implement this soon
struct PowerInteractionBoolHolder {
	bool shouldDie;
	bool otherShouldDie;
};

class InheritedPowerCommon {
protected:
public:
	double timeLeft;
	double maxTime;

	//ColorValueHolder color;

public:
	//virtual void initialize(Tank* parent);
	//virtual void removePower(Tank* parent);

	virtual void tick() = 0;
	virtual void powerTick() = 0;

	virtual ColorValueHolder getColor() = 0;

	//void (*modifiedMovement)();
	virtual bool isDone() = 0; //as in, is finished, shouldn't exist anymore

	//don't use function pointers; too complicated, don't hold a this (didn't try member function pointers for too long, but probably not worth the effort)
	//void (*modifiedCollisionWithTank)(Tank*); //something might use it
	//void (*modifiedCollisionWithWall)(Wall*); //wallhack and bounce
	//virtual void modifiedCollisionWithPower(Power*); //probably not going to be used
	//virtual void modifiedCollisionWithBullet(Bullet*); //probably shouldn't be used
	//virtual void modifiedCollisionWithHazard(Hazard*);

	//virtual void modifiedShooting();

	virtual double getOffenseImportance() { return 0; } //"importance" = "override" value (when dealing with other powers)
	virtual double getOffenseTier() { return 0; }
	virtual double getDefenseImportance() { return 0; }
	virtual double getDefenseTier() { return 0; }
	//removed values to the tiers because they probably wouldn't get used
};