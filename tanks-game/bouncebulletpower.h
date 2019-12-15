#pragma once
class BounceBulletPower;

#include "power.h"
#include "bulletpower.h"
#include "bouncepower.h"

class BounceBulletPower : public BulletPower {
protected:
public:
	static const short maxBounces;
	short bouncesLeft;

public:
	virtual void initialize(Bullet* parent);
	virtual void removeEffects(Bullet* parent);

	virtual void tick() { return; }
	virtual ColorValueHolder getColor() {
		return BouncePower::getClassColor();
	}

	virtual TankPower* makeTankPower();
	
	//bool modifiesCollisionWithWall = true;
	virtual bool modifiedCollisionWithWall(Bullet*, Wall*);

	//bool modifiesCollisionWithEdge = true;
	virtual bool modifiedEdgeCollision(Bullet*);

	//virtual double getOffenseTier() { return 0; }
	//virtual double getOffenseValue() { return 0; }
	//virtual double getDefenseTier() { return 0; }
	//virtual double getDefenseValue() { return 0; }

	BounceBulletPower();
};