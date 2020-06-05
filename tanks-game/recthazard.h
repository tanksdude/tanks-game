#pragma once
class RectHazard;

#include "hazard.h"
#include "rect.h"
#include <string>
//#include "recthazardpower.h"
#include "tank.h"
#include "bullet.h"

//this is basically so lightning can get randomized (currently unused)
struct RectFactoryInformation {
	bool requiresWallOnLeft;
	bool requiresWallOnRight;
	bool requiresWallOnTop;
	bool requiresWallOnBottom;
	bool wallRequirementCanBeAnEdge;
	RectFactoryInformation(bool wallLeft, bool wallRight, bool wallTop, bool wallBottom, bool edgeEqualsWall) {
		requiresWallOnLeft = wallLeft;
		requiresWallOnRight = wallRight;
		requiresWallOnTop = wallTop;
		requiresWallOnBottom = wallBottom;
		wallRequirementCanBeAnEdge = edgeEqualsWall;
	}
	RectFactoryInformation(bool wallLeft, bool wallRight, bool wallTop, bool wallBottom)
	: RectFactoryInformation(wallLeft, wallRight, wallTop, wallBottom, true) {}
};

//this is intended for hazard randomization; if the actual hazard type is known, then the constructor will be known
enum class RectHazardConstructionTypes /*: unsigned char */ {
	constructionIsTooComplex, //I'm thinking pipes would have this
	simpleConstruction, //only x and y required (what would use this? fake powerups?)
	standardConstruction //x, y, w, and h required
};

class RectHazard : public Hazard, public Rect {
public: //protected?
	//std::vector<RectHazardPower*> hazardPowers;
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

	static RectHazard* factory(int argc, std::string* argv);
	static int getFactoryArgumentCount();
	//static RectHazardConstructionTypes getConstructionType();
	virtual RectFactoryInformation getFactoryInformation() {
		return { false, false, false, false, false };
	}
};
