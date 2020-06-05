#pragma once
#include "gamething.h"
#include "colorvalueholder.h"

class Hazard : public GameThing {
	friend class ResetThings;
protected:
	bool canAcceptPowers;

public:
	//virtual ColorValueHolder getColor();
	//virtual bool getCanAcceptPowers();
	virtual bool validLocation() { return true; } //false if unable to operate properly where it is; "operate properly" is a little vague
	virtual bool reasonableLocation() = 0;

	virtual void tick() = 0;
	virtual void draw() = 0;
	//virtual void drawCPU();
};
//should each hazard have a function that adds X amount of itself randomly to the level?)

/*
some hazards aren't bad

list of (eventual) hazards:
|stationary turret
turret
patrolling turret
generalized lightning (has point to point bolt; in order for it to work for circular lightning, a function would have to be passed in (function checks whether point is in the lightning))
|horizontal lightning
vertical lightning
lightning zone (rectangle)
lightning circle
|rectangular lava
|circular lava
no bullet zone
teleport zone
pipes


list of possible hazards, joke or not:
the reflecktor (bounces/reflects tanks when they hit it)

*/
