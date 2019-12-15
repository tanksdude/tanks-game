#pragma once
#include "bullet.h"

class BulletPower {
protected:
	double timeLeft;
	double maxTime; //bullet powers typically last forever, so they should default to -1

public:
	virtual void initialize(Bullet* parent);
	virtual void removePower(Bullet* parent);

	virtual void tick();

	virtual void modifiedMovement();

	virtual void modifiedCollisionWithTank();
	virtual void modifiedCollisionWithWall();
	virtual void modifiedCollisionWithPower(); //probably not going to be used
	virtual void modifiedCollisionWithBullet(); //probably shouldn't be used
	//virtual void modifiedCollisionWithHazard();

	virtual void modifiedTankDrawings();

	virtual double getOffenseTier(); //don't want it to be a variable because a function can change its value much easier
	virtual double getOffenseValue(); //only one tier per power
	virtual double getDefenseTier();
	virtual double getDefenseValue(); //return 0 by default, else 1 probably
};