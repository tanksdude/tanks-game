#pragma once
#include "../circle-hazard.h"

#include "../constants.h"

#include "../simple-vector-2d.h"

class StationaryTurretHazard : public CircleHazard {
	//just called Stationary in JS Tanks
protected:
	SimpleVector2D velocity; //for stationary and targeting turrets, the magnitude will obviously be 0
	double tickCount;
	double tickCycle;
	unsigned int currentState;
	unsigned int maxState;
	double* stateMultiplier;
	ColorValueHolder* stateColors;

protected:
	virtual bool canSeeTank(const Tank*) const; //true if pointing at tank with no wall obstructions
	virtual ColorValueHolder getColor() const; //needed because turret doesn't use tickCount, instead using targetingCount (should change this)
	virtual ColorValueHolder getColor(int state) const;

public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual CircleHazardCollisionType getCollisionType() const override { return CircleHazardCollisionType::solid; }

protected:
	virtual float getDefaultOffense() const override { return 0; }
	virtual float getDefaultDefense() const override { return DESTRUCTION_TIER; }

public:
	//virtual bool validLocation() const override { return true; }
	virtual bool reasonableLocation() const override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "stationary_turret"; }

	virtual void tick() override;
	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

protected:
	virtual inline void drawBody(float alpha = 1.0f) const;
	virtual inline void drawOutline(float alpha = 1.0f) const;
	virtual inline void drawBarrel(float alpha = 1.0f) const;

protected:
	static SimpleVector2D body_vertices[Circle::numOfSides+1];
	static unsigned int body_indices[Circle::numOfSides*3];
	static unsigned int outline_indices[Circle::numOfSides*2*3];
	static bool initialized_vertices;

	static bool initializeVertices();

public:
	StationaryTurretHazard(double xpos, double ypos, double angle);
	virtual ~StationaryTurretHazard();
	static CircleHazard* factory(const GenericFactoryConstructionData&);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
	virtual int getFactoryArgumentCount() const override { return 3; }
	virtual CircleHazardConstructionTypes getConstructionType() const override { return CircleHazardConstructionTypes::angleRequired; }
	virtual CircleFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
