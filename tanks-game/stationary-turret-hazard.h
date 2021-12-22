#pragma once
#include "circle-hazard.h"
#include "simple-vector-2d.h"
#include "constants.h"

#include "vertex-array.h"
#include "vertex-buffer.h"
#include "index-buffer.h"

class StationaryTurretHazard : public CircleHazard {
	//just called Stationary in JS Tanks
protected:
	SimpleVector2D direction;
	double tickCount;
	double tickCycle;
	unsigned int currentState;
	unsigned int maxState;
	double* stateMultiplier;
	ColorValueHolder* stateColors;

protected:
	//double getAngle() const;
	virtual bool canSeeTank(const Tank*) const; //true if pointing at tank with no wall obstructions
	virtual ColorValueHolder getColor() const; //needed because turret doesn't use tickCount, instead using targetingCount (should change this)
	virtual ColorValueHolder getColor(int state) const;

private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
	static VertexArray* cannon_va;
	static VertexBuffer* cannon_vb;
	static bool initialized_GPU;

	static bool initializeGPU();
	static bool uninitializeGPU();

public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual CircleHazardCollisionType getCollisionType() const override { return CircleHazardCollisionType::solid; }

protected:
	virtual double getDefaultOffense() const override { return 0; }
	virtual double getDefaultDefense() const override { return DESTRUCTION_TIER; }

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
	//virtual void drawCPU() const;

private:
	inline void drawBody(float alpha = 1.0f) const;
	inline void drawOutline(float alpha = 1.0f) const;
	inline void drawBarrel(float alpha = 1.0f) const;

protected:
	StationaryTurretHazard(double xpos, double ypos, double angle, bool noGPU); //doesn't initialize GPU
public:
	StationaryTurretHazard(double xpos, double ypos, double angle);
	StationaryTurretHazard(double xpos, double ypos, double angle, double radius);
	virtual ~StationaryTurretHazard();
	static CircleHazard* factory(int, std::string*);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv);
	virtual int getFactoryArgumentCount() const override { return 3; }
	virtual CircleHazardConstructionTypes getConstructionType() const override { return CircleHazardConstructionTypes::angleRequired; }
	virtual CircleFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
