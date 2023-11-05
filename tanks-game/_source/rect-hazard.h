#pragma once
class RectHazard;

#include <string>
#include <vector>
#include <unordered_map>

#include "game-thing.h"
#include "rect.h"
#include "drawable-thing.h"
//#include "rect-hazard-power.h"

#include "tank.h"
#include "bullet.h"
#include "generic-factory-construction-data.h"

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
	standardConstruction, //x, y, w, and h required
	standardSquareConstruction, //x, y, and w required
};

enum class RectHazardCollisionType {
	solid,
	under
};

class RectHazard : public GameThing, public Rect, public DrawableThing {
public: //protected?
	//std::vector<RectHazardPower*> hazardPowers;
	//bool canAcceptPowers;
	//virtual bool getCanAcceptPowers();

public:
	float getOffenseTier() const;
	float getDefenseTier() const;

protected:
	float getHighestOffenseImportance() const;
	float getHighestOffenseTier(float importance) const;
	float getHighestDefenseImportance() const;
	float getHighestDefenseTier(float importance) const;

public:
	virtual std::vector<std::string> getHazardTypes() const = 0; //pure virtual to make sure recthazards define it
	virtual std::unordered_map<std::string, float> getWeights() const = 0; //intended range: (0,1]

	virtual RectHazardCollisionType getCollisionType() const = 0; // { return RectHazardCollisionType::solid; }

	virtual bool actuallyCollided(const Tank*) const { return true; } //precondition: currently and partially collided with tank
	bool modifiesTankCollision = false;
	virtual void modifiedTankCollision(Tank*);
	bool hasSpecialEffectTankCollision = false;
	virtual void specialEffectTankCollision(const Tank*) { return; } //always activated before modifiedTankCollision

	virtual bool actuallyCollided(const Bullet*) const { return true; } //precondition: currently and partially collided with bullet
	bool modifiesBulletCollision = false;
	virtual void modifiedBulletCollision(Bullet*);
	bool hasSpecialEffectBulletCollision = false;
	virtual void specialEffectBulletCollision(const Bullet*) { return; } //always activated before modifiedBulletCollision

protected:
	virtual float getDefaultOffense() const = 0;
	virtual float getDefaultDefense() const = 0;

public:
	virtual bool validLocation() const { return true; }
	virtual bool reasonableLocation() const = 0;
	virtual void initialize() { return; } //called when recthazard is pushed for the first time

	virtual std::string getName() const = 0;
	//static std::string getClassName();

	virtual void tick() = 0;
	virtual void draw() const override = 0;
	virtual void draw(DrawingLayers) const override = 0;
	virtual void poseDraw() const override = 0;
	virtual void poseDraw(DrawingLayers) const override = 0;
	virtual void ghostDraw(float alpha) const override = 0;
	virtual void ghostDraw(DrawingLayers, float alpha) const override = 0;

	virtual ~RectHazard() { return; }
	static RectHazard* factory(const GenericFactoryConstructionData&);
	static RectHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
	virtual int getFactoryArgumentCount() const = 0;
	virtual RectHazardConstructionTypes getConstructionType() const = 0;
	virtual RectFactoryInformation getFactoryInformation() const = 0;

protected:
	RectHazard(Team_ID t_id) : GameThing(t_id) {}
};
