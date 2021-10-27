#pragma once
#include "circlehazard.h"
#include "constants.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class StationaryTurret : public CircleHazard {
	//just called Stationary in JS Tanks
protected:
	double angle;
	double tickCount;
	double tickCycle;
	unsigned short currentState; //could use int instead of short because transfering a word is more efficient than a halfword but... eh
	unsigned short maxState;
	double* stateMultiplier;
	ColorValueHolder* stateColors;

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
	virtual double getDefaultOffense() override { return 0; }
	virtual double getDefaultDefense() override { return DESTRUCTION_TIER; }
	//needs some sort of "overriding priority" function to destroy bullets with offense less than this defense

	//virtual bool validLocation() override { return true; }
	virtual bool reasonableLocation() override;

	double getAngle() const;
	virtual bool canSeeTank(Tank*) const; //true if pointing at tank with no wall obstructions
	virtual ColorValueHolder getColor() const; //needed because turret doesn't use tickCount, instead using targetingCount (should change this)
	virtual ColorValueHolder getColor(short state) const;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "stationary_turret"; }

	virtual void tick() override;
	virtual void draw() const override;
	virtual void draw(double xpos, double ypos) const override;
	virtual void poseDraw() const override;
	virtual void drawCPU();

protected:
	StationaryTurret(double xpos, double ypos, double angle, bool noGPU); //doesn't initialize GPU
public:
	StationaryTurret(double xpos, double ypos, double angle);
	StationaryTurret(double xpos, double ypos, double angle, double radius);
	virtual ~StationaryTurret();
	static CircleHazard* factory(int, std::string*);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv);
	virtual int getFactoryArgumentCount() const override { return 3; }
	virtual CircleHazardConstructionTypes getConstructionType() const override { return CircleHazardConstructionTypes::angleRequired; }
	virtual CircleFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
