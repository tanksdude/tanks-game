#pragma once
class CircleHazard;

#include "hazard.h"
#include "circle.h"
#include <string>

//#include "circlehazardpower.h"
#include "tank.h"
#include "bullet.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class CircleHazard : public Hazard, public Circle {
public: //protected?
	//std::vector<CircleHazardPower*> hazardPowers;
	double getHighestOffenseImportance();
	double getHighestOffenseTier(double importance);
	double getHighestDefenseImportance();
	double getHighestDefenseTier(double importance);
public:
	double getOffenseTier();
	double getDefenseTier();
	virtual double getDefaultOffense() = 0;
	virtual double getDefaultDefense() = 0;

	bool modifiesTankCollision = false;
	virtual bool actuallyCollided(Tank*) { return true; } //precondition: currently and partially collided with tank
	virtual void modifiedTankCollision(Tank*);

	bool modifiesBulletCollision = false;
	virtual bool actuallyCollided(Bullet*) { return true; } //precondition: currently and partially collided with bullet
	virtual void modifiedBulletCollision(Bullet*);

	virtual std::string getName() = 0;
	static std::string getClassName();
	//virtual bool initializeGPU() = 0;
	virtual void draw() = 0;

	static CircleHazard* factory(int argc, std::string* argv); //strings so any data type can be used (theoretically; structs can't, ya know)
};