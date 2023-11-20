#pragma once
#include "stationary-turret-hazard.h"

class TargetingTurretHazard : public StationaryTurretHazard {
	//just called Turret in JS Tanks
protected:
	double turningIncrement = 128;
	bool targeting;
	float targetingX, targetingY; //not the x and y of the targeted tank; it's the x and y of the targeting reticule
	ColorValueHolder reticuleColors[2];
	double targetingCount;
	Game_ID trackingID; //if ==this->getGameID(), then it's not tracking

protected:
	virtual inline void updateTrackingPos(const Tank*, bool pointedAt);

	virtual void turnTowardsTank(const Tank*);
	virtual bool canSeeTank(const Tank*) const override; //true if no walls obstruct any line of sight to tank
	virtual bool isPointedAt(const Tank*) const;
	virtual ColorValueHolder getColor() const override;
	virtual ColorValueHolder getReticuleColor() const;

public:
	//virtual std::vector<std::string> getHazardTypes() const override; //same as StationaryTurretHazard
	virtual std::unordered_map<std::string, float> getWeights() const override;

public:
	virtual bool reasonableLocation() const override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "targeting_turret"; }

	virtual void tick() override;
	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

protected:
	virtual inline void tick_continueTracking();
	virtual inline void tick_lookForNewTarget();
	virtual inline void tick_chargeUp();
	virtual inline void tick_cooldown();

protected:
	virtual inline void drawReticule(float alpha = 1.0f) const;

public:
	TargetingTurretHazard(double xpos, double ypos, double angle);
	virtual ~TargetingTurretHazard();
	static CircleHazard* factory(const GenericFactoryConstructionData&);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
};
