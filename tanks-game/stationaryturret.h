#pragma once
#include "circlehazard.h"
#include <math.h>
#include "constants.h"

class StationaryTurret : public CircleHazard {
protected:
	double angle;
	double tickCount = 0;
	double tickCycle;
	unsigned short currentState = 0; //could use int instead of short because transfering a word is more efficient than a halfword but... eh
	unsigned short maxState;
	short* stateMultiplier;
	ColorValueHolder* stateColors;

public:
	virtual ColorValueHolder getColor();
	virtual ColorValueHolder getColor(short state);
	virtual std::string getName() { return getClassName(); };
	static std::string getClassName() { return "stationary turret"; };
	double getAngle() { return fmod(fmod(angle, 2*PI) + 2*PI, 2*PI); }

	void tick();
	void draw();
	void drawCPU();
	StationaryTurret(double xpos, double ypos, double angle);
	StationaryTurret(double xpos, double ypos, double angle, double radius);
	~StationaryTurret();
	static CircleHazard* factory(int, std::string*);
};