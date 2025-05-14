#pragma once
#include "targeting-turret-hazard.h"

class MotherTurretHazard : public TargetingTurretHazard {
protected:
	int maxChildTurrets; //treated as constant
	std::vector<Game_ID> childTurretIDs; //length = maxChildTurrets (vector for ease of use, can't use std::array)
	std::vector<bool> childTurretAlive;
	int targetingNum; //[0, maxChildTurrets)
	bool targetingChild; //basically the targeting bool, but the distinction allows GinormousTurretHazard to work correctly

	double childDistFromMother;
	float initialAngle;
	int initialChildren;

protected:
	virtual void pushInitialChildren(int count);

	int getChildCount() const; //update child count before using this!
	int updateChildCount(); //returns alive children
	virtual CircleHazard* makeTurret(int turretNum) const;
	void pushChild(int turretNum); //shouldn't need to be virtual
	float getChildTurretAngle(int turretNum) const; //same

	virtual void turnTowardsPoint(int turretNum);
	virtual bool isPointedAt(int turretNum) const;

public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual float getOffenseTier() const override { return 0; }
	virtual float getDefenseTier() const override { return DESTRUCTION_TIER + .5f; } //TODO: double big bullets should be able to destroy this

public:
	virtual bool reasonableLocation() const override;
	virtual void initialize() override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "mother_turret"; }

	virtual void tick() override;
	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

protected:
	virtual void tick_chooseSpot();
	virtual void tick_trackSpot();
	virtual void tick_chargeUp() override;

protected:
	virtual void drawShootingTimer(float alpha = 1.0f) const;
	virtual void drawChildTurretLocations(float alpha = 1.0f) const; //debug

public:
	MotherTurretHazard(double xpos, double ypos, double angle);
	MotherTurretHazard(double xpos, double ypos, double angle, int maxChildren, int startChildren);
	MotherTurretHazard(double xpos, double ypos, double angle, int maxChildren, int startChildren, double childDistMultiplier); //multiplier of mother radius
	virtual ~MotherTurretHazard();
	static CircleHazard* factory(const GenericFactoryConstructionData&);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
};
