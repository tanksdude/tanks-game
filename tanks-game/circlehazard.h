#pragma once
class CircleHazard;

#include "hazard.h"
#include "circle.h"
#include <string>

//#include "circlehazardpower.h"
#include "tank.h"
#include "bullet.h"

//this is intended for hazard randomization; if the actual hazard type is known, then the constructor will be known
enum class CircleHazardConstructionTypes /*: unsigned char */ {
	constructionIsTooComplex, //I'm thinking patrolling turret would have this
	simpleConstruction, //only x and y required (no r or angle)
	angleRequired,
	radiusRequired,
	radiusAndAngleRequired
};

class CircleHazard : public Hazard, public Circle {
public: //protected?
	//std::vector<CircleHazardPower*> hazardPowers;
public:
	double getHighestOffenseImportance();
	double getHighestOffenseTier(double importance);
	double getHighestDefenseImportance();
	double getHighestDefenseTier(double importance);
public:
	double getOffenseTier();
	double getDefenseTier();
	virtual double getDefaultOffense() = 0;
	virtual double getDefaultDefense() = 0;

	virtual bool actuallyCollided(Tank*) { return true; } //precondition: currently and partially collided with tank
	bool modifiesTankCollision = false;
	virtual void modifiedTankCollision(Tank*);
	bool hasSpecialEffectTankCollision = false;
	virtual void specialEffectTankCollision(Tank*) { return; } //always activated before modifiedTankCollision

	virtual bool actuallyCollided(Bullet*) { return true; } //precondition: currently and partially collided with bullet
	bool modifiesBulletCollision = false;
	virtual void modifiedBulletCollision(Bullet*);
	bool hasSpecialEffectBulletCollision = false;
	virtual void specialEffectBulletCollision(Bullet*) { return; } //always activated before modifiedBulletCollision

	//virtual bool validLocation() { return true; }
	//virtual bool reasonableLocation() = 0;

	virtual std::string getName() = 0;
	static std::string getClassName();
	//virtual bool initializeGPU() = 0;
	virtual void draw() = 0;

	static CircleHazard* factory(int argc, std::string* argv);
	static int getFactoryArgumentCount();
	//static CircleHazardConstructionTypes getConstructionType();
};
