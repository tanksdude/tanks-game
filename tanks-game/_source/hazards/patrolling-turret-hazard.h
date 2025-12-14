#pragma once
#include "targeting-turret-hazard.h"

//TODO: patrolling turrets should be affected by walls
class PatrollingTurretHazard : public TargetingTurretHazard {
protected:
	double* routePosList;
	int routePosPairNum;
	int currentPatrolTarget;
	double waitCount;
	double* routeWaitCount;

protected:
	inline double getRoutePosX(int i) const;
	inline double getRoutePosY(int i) const;

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
	virtual void tick_lookForNewTarget() override;

	virtual void tick_patrol();
	virtual void tick_moveForward();
	virtual void tick_patrolWait();

protected:
	virtual void drawPath(float alpha = 1.0f) const; //debug

public:
	PatrollingTurretHazard(double xpos, double ypos, double angle, int pairNum, const double* posList, const double* waitList);
	virtual ~PatrollingTurretHazard();
	static CircleHazard* factory(const GenericFactoryConstructionData&);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
	virtual int getFactoryArgumentCount() const override { return 5; }
	virtual CircleHazardConstructionTypes getConstructionType() const override { return CircleHazardConstructionTypes::constructionIsTooComplex; }
	virtual CircleFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
