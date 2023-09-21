#pragma once
#include "targeting-turret-hazard.h"

class MotherTurretHazard : public TargetingTurretHazard {
protected:
	//SimpleVector2D velocity; //for stationary and targeting turrets, the magnitude will obviously be 0
	//double tickCount;
	//double tickCycle;
	//unsigned int currentState;
	//unsigned int maxState;
	//double* stateMultiplier;
	//ColorValueHolder* stateColors;

	//double turningIncrement = 128;
	//bool targeting;
	//double targetingX, targetingY; //not the x and y of the targeted tank; it's the x and y of the targeting reticule
	//ColorValueHolder reticuleColors[2];
	//double targetingCount;
	//Game_ID trackingID; //if ==this->getGameID(), then it's not tracking

	int maxChildTurrets; //treated as constant
	std::vector<Game_ID> childTurretIDs; //length = maxChildTurrets (vector for ease of use)
	std::vector<bool> childTurretAlive;
	int targetingNum; //[0, maxChildTurrets)
	bool targetingChild; //basically the targeting bool, but the distinction allows GinormousTurretHazard to work correctly

	double childDistFromMother;
	double initialAngle;
	int initialChildren;

protected:
	virtual inline void pushInitialChildren(int count);

	inline int getChildCount() const; //update child count before using this!
	int updateChildCount(); //returns alive children
	virtual CircleHazard* makeTurret(int turretNum) const;
	void pushChild(int turretNum); //shouldn't need to be virtual
	inline double getChildTurretAngle(int turretNum) const; //same

	virtual void turnTowardsPoint(int turretNum);
	virtual bool isPointedAt(int turretNum) const;

public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

protected:
	virtual float getDefaultOffense() const override { return 0; }
	virtual float getDefaultDefense() const override { return DESTRUCTION_TIER + .5; } //TODO: double big bullets should be able to destroy this

public:
	//virtual bool validLocation() const override { return true; }
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
	//virtual inline void tick_continueTracking() override;
	//virtual inline void tick_lookForNewTarget() override;
	virtual inline void tick_chooseSpot();
	virtual inline void tick_trackSpot();
	virtual inline void tick_chargeUp() override;
	virtual inline void tick_cooldown() override;

protected:
	virtual inline void drawShootingTimer(float alpha = 1.0f) const;
	virtual inline void drawChildTurretLocations(float alpha = 1.0f) const; //debug

public:
	MotherTurretHazard(double xpos, double ypos, double angle);
	MotherTurretHazard(double xpos, double ypos, double angle, int maxChildren, int startChildren);
	MotherTurretHazard(double xpos, double ypos, double angle, int maxChildren, int startChildren, double childDistMultiplier); //multiplier of mother radius
	virtual ~MotherTurretHazard();
	static CircleHazard* factory(const GenericFactoryConstructionData&);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
	//virtual int getFactoryArgumentCount() const override { return 3; }
	//virtual CircleHazardConstructionTypes getConstructionType() const override { return CircleHazardConstructionTypes::angleRequired; }
	//virtual CircleFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
