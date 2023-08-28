#pragma once
#include "targeting-turret-hazard.h"

//TODO: patrolling turrets should be affected by walls
class PatrollingTurretHazard : public TargetingTurretHazard {
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

	double* routePosList;
	int routePosPairNum;
	int currentPatrolTarget;
	double waitCount;
	double* routeWaitCount;

protected:
	//virtual inline void updateTrackingPos(const Tank*, bool pointedAt);

	//virtual void turnTowardsTank(const Tank*);
	//virtual bool canSeeTank(const Tank*) const override; //true if no walls obstruct any line of sight to tank
	//virtual bool isPointedAt(const Tank*) const;
	//virtual ColorValueHolder getColor() const override;
	//virtual ColorValueHolder getColor(int state) const override;
	//virtual ColorValueHolder getReticuleColor() const;

	virtual inline double getRoutePosX(int i) const;
	virtual inline double getRoutePosY(int i) const;

	virtual bool isCloseAsPossibleToCurrentPoint() const;
	virtual bool isWaitingAtPoint() const;
	virtual void turnTowardsPoint();
	virtual bool isPointedAtPoint() const;

public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

public:
	//virtual bool validLocation() const override { return true; }
	virtual bool reasonableLocation() const override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "patrolling_turret"; }

	virtual void tick() override;
	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

protected:
	//virtual inline void tick_continueTracking();
	virtual inline void tick_lookForNewTarget() override;
	//virtual inline void tick_chargeUp();
	//virtual inline void tick_cooldown();

	virtual inline void tick_patrol();
	virtual inline void tick_moveForward();
	virtual inline void tick_patrolWait();

private:
	inline void drawBody(float alpha = 1.0f) const;
	inline void drawOutline(float alpha = 1.0f) const;
	inline void drawBarrel(float alpha = 1.0f) const;
	inline void drawReticule(float alpha = 1.0f) const;
	inline void drawPath(float alpha = 1.0f) const; //debug

public:
	PatrollingTurretHazard(double xpos, double ypos, double angle, int pairNum, double* posList, double* waitList);
	virtual ~PatrollingTurretHazard();
	static CircleHazard* factory(GenericFactoryConstructionData&);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, GenericFactoryConstructionData&);
	virtual int getFactoryArgumentCount() const override { return 5; }
	virtual CircleHazardConstructionTypes getConstructionType() const override { return CircleHazardConstructionTypes::constructionIsTooComplex; }
	virtual CircleFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
