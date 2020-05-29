#pragma once
#include "circlehazard.h"
#include <math.h>
#include "constants.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class StationaryTurret : public CircleHazard {
protected:
	double angle;
	double tickCount = 0;
	double tickCycle;
	unsigned short currentState = 0; //could use int instead of short because transfering a word is more efficient than a halfword but... eh
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
public:
	static bool initializeGPU();
	static bool uninitializeGPU();

public:
	virtual double getDefaultOffense() { return 0; }
	virtual double getDefaultDefense() { return DESTRUCTION_TIER; }
	//needs some sort of "overriding priority" function to destroy bullets with offense less than this defense

	//virtual bool validLocation() { return true; }
	virtual bool reasonableLocation();

	double getAngle() { return fmod(fmod(angle, 2*PI) + 2*PI, 2*PI); }
	virtual ColorValueHolder getColor();
	virtual ColorValueHolder getColor(short state);

	virtual std::string getName() { return getClassName(); }
	static std::string getClassName() { return "stationary turret"; }

	virtual void tick();
	virtual void draw();
	virtual void drawCPU();

	StationaryTurret(double xpos, double ypos, double angle);
	StationaryTurret(double xpos, double ypos, double angle, double radius);
	~StationaryTurret();
	static CircleHazard* factory(int, std::string*);
	static int getFactoryArgumentCount() { return 3; }
	//static CircleHazardConstructionTypes getConstructionType() { return CircleHazardConstructionTypes::angleRequired; }
};
