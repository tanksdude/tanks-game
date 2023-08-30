#pragma once
#include "stationary-turret-hazard.h"

class TargetingTurretHazard : public StationaryTurretHazard {
	//just called Turret in JS Tanks
protected:
	//SimpleVector2D velocity; //for stationary and targeting turrets, the magnitude will obviously be 0
	//double tickCount;
	//double tickCycle;
	//unsigned int currentState;
	//unsigned int maxState;
	//double* stateMultiplier;
	//ColorValueHolder* stateColors;

	double turningIncrement = 128;
	bool targeting;
	double targetingX, targetingY; //not the x and y of the targeted tank; it's the x and y of the targeting reticule
	ColorValueHolder reticuleColors[2];
	double targetingCount;
	Game_ID trackingID; //if ==this->getGameID(), then it's not tracking

protected:
	virtual inline void updateTrackingPos(const Tank*, bool pointedAt);

	virtual void turnTowardsTank(const Tank*);
	virtual bool canSeeTank(const Tank*) const override; //true if no walls obstruct any line of sight to tank
	virtual bool isPointedAt(const Tank*) const;
	virtual ColorValueHolder getColor() const override;
	virtual ColorValueHolder getColor(int state) const override;
	virtual ColorValueHolder getReticuleColor() const;

public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

public:
	//virtual bool validLocation() const override { return true; }
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

private:
	inline void drawBody(float alpha = 1.0f) const;
	inline void drawOutline(float alpha = 1.0f) const;
	inline void drawBarrel(float alpha = 1.0f) const;
	inline void drawReticule(float alpha = 1.0f) const;

public:
	TargetingTurretHazard(double xpos, double ypos, double angle);
	TargetingTurretHazard(double xpos, double ypos, double angle, double radius);
	virtual ~TargetingTurretHazard();
	static CircleHazard* factory(GenericFactoryConstructionData&);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, GenericFactoryConstructionData&);
	//virtual int getFactoryArgumentCount() const override { return 3; }
	//virtual CircleHazardConstructionTypes getConstructionType() const override { return CircleHazardConstructionTypes::angleRequired; }
	//virtual CircleFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
