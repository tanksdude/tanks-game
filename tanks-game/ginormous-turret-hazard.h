#pragma once
#include "targeting-turret-hazard.h"

class GinormousTurretHazard : public TargetingTurretHazard {
	//was thinking about calling it "god turret," but this turret just doesn't do enough
	//could probably extend stationary turret
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

	int bulletCount;

protected:
	virtual ColorValueHolder getColor() const override;
	virtual ColorValueHolder getColor(int state) const override;

public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

protected:
	virtual double getDefaultOffense() const override { return 0; }
	virtual double getDefaultDefense() const override { return DESTRUCTION_TIER + 1.0; }

public:
	//virtual bool validLocation() const override { return true; }
	virtual bool reasonableLocation() const override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "ginormous_turret"; }

	virtual void tick() override;
	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

private:
	inline void drawBody(float alpha = 1.0f) const;
	inline void drawOutline(float alpha = 1.0f) const;
	inline void drawBarrel(float alpha = 1.0f) const;

public:
	GinormousTurretHazard(double xpos, double ypos, double angle);
	virtual ~GinormousTurretHazard();
	static CircleHazard* factory(const GenericFactoryConstructionData&);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
	//virtual int getFactoryArgumentCount() const override { return 3; }
	//virtual CircleHazardConstructionTypes getConstructionType() const override { return CircleHazardConstructionTypes::angleRequired; } //probably doesn't need an angle...
	//virtual CircleFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
