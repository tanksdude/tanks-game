#pragma once
class CircleHazard;

#include "hazard.h"
#include "circle.h"
#include <string>
#include <unordered_map>
//#include "circlehazardpower.h"
#include "tank.h"
#include "bullet.h"

//I strongly doubt anything will use this; the only thing I can think of is like a sentry gun, so I'm probably going to get rid of this
struct CircleFactoryInformation {
	bool requiresWallOnLeft;
	bool requiresWallOnRight;
	bool requiresWallOnTop;
	bool requiresWallOnBottom;
	bool wallRequirementCanBeAnEdge;
	CircleFactoryInformation(bool wallLeft, bool wallRight, bool wallTop, bool wallBottom, bool edgeEqualsWall) {
		requiresWallOnLeft = wallLeft;
		requiresWallOnRight = wallRight;
		requiresWallOnTop = wallTop;
		requiresWallOnBottom = wallBottom;
		wallRequirementCanBeAnEdge = edgeEqualsWall;
	}
	CircleFactoryInformation(bool wallLeft, bool wallRight, bool wallTop, bool wallBottom)
	: CircleFactoryInformation(wallLeft, wallRight, wallTop, wallBottom, true) {}
	CircleFactoryInformation()
	: CircleFactoryInformation(false, false, false, false) {}
};

//this is intended for hazard randomization; if the actual hazard type is known, then the constructor will be known
enum class CircleHazardConstructionTypes /*: unsigned char */ {
	constructionIsTooComplex, //I'm thinking patrolling turret would have this (add a "path required" parameter?)
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
	virtual std::vector<std::string> getHazardTypes();
	virtual std::unordered_map<std::string, float> getWeights(); //intended range: (0,1]

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
	//static std::string getClassName();
	//virtual bool initializeGPU() = 0;
	virtual void draw() = 0;

	static CircleHazard* factory(int argc, std::string* argv);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv);
	virtual int getFactoryArgumentCount() = 0;
	virtual CircleHazardConstructionTypes getConstructionType() = 0;
	virtual CircleFactoryInformation getFactoryInformation() = 0;
	virtual ~CircleHazard() { return; }
};
