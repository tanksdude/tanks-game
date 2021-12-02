#pragma once
class RectHazard;

#include "hazard.h"
#include "drawablething.h"
#include "rect.h"
#include <string>
#include <unordered_map>
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
	RectFactoryInformation()
	: RectFactoryInformation(false, false, false, false) {}
};

//this is intended for hazard randomization; if the actual hazard type is known, then the constructor will be known
enum class RectHazardConstructionTypes /*: unsigned char */ {
	constructionIsTooComplex, //I'm thinking pipes would have this
	simpleConstruction, //only x and y required (what would use this? fake powerups?)
	standardConstruction //x, y, w, and h required
};

enum class RectHazardCollisionType {
	solid,
	under
};

class RectHazard : public Hazard, public Rect, public DrawableThing {
public: //protected?
	//std::vector<RectHazardPower*> hazardPowers;

public:
	double getOffenseTier() const;
	double getDefenseTier() const;

protected:
	double getHighestOffenseImportance() const;
	double getHighestOffenseTier(double importance) const;
	double getHighestDefenseImportance() const;
	double getHighestDefenseTier(double importance) const;

public:
	virtual std::vector<std::string> getHazardTypes() const = 0; //pure virtual to make sure recthazards define it
	virtual std::unordered_map<std::string, float> getWeights() const = 0; //intended range: (0,1]

	virtual bool actuallyCollided(const Tank*) const { return true; } //precondition: currently and partially collided with tank
	bool modifiesTankCollision = false;
	virtual void modifiedTankCollision(Tank*);
	bool hasSpecialEffectTankCollision = false;
	virtual void specialEffectTankCollision(Tank*) { return; } //always activated before modifiedTankCollision

	virtual bool actuallyCollided(const Bullet*) const { return true; } //precondition: currently and partially collided with bullet
	bool modifiesBulletCollision = false;
	virtual void modifiedBulletCollision(Bullet*);
	bool hasSpecialEffectBulletCollision = false;
	virtual void specialEffectBulletCollision(Bullet*) { return; } //always activated before modifiedBulletCollision

public:
	virtual bool validLocation() const override { return true; }
	virtual bool reasonableLocation() const override = 0;

	virtual std::string getName() const = 0;
	//static std::string getClassName();

	virtual double getDefaultOffense() const = 0;
	virtual double getDefaultDefense() const = 0;
	virtual RectHazardCollisionType getCollisionType() const = 0; // { return RectHazardCollisionType::solid; }

	virtual void tick() override = 0;
	virtual void draw() const override = 0;
	virtual void draw(DrawingLayers) const override = 0;
	virtual void poseDraw() const override = 0;
	virtual void poseDraw(DrawingLayers) const override = 0;
	virtual void ghostDraw(float alpha) const override = 0;
	virtual void ghostDraw(DrawingLayers, float alpha) const override = 0;

	virtual ~RectHazard() { return; }
	static RectHazard* factory(int argc, std::string* argv);
	static RectHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv);
	virtual int getFactoryArgumentCount() const = 0;
	virtual RectHazardConstructionTypes getConstructionType() const = 0;
	virtual RectFactoryInformation getFactoryInformation() const = 0;
};
