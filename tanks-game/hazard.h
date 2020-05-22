#pragma once
#include "gamething.h"
#include "colorvalueholder.h"

class Hazard : public GameThing {
	friend class ResetThings;
protected:
	bool canAcceptPowers;
	//std::vector<HazardPower*> hazardPowers;

public:
	//virtual ColorValueHolder getColor();
	//virtual bool getCanAcceptPowers();

	virtual void tick() = 0;
	virtual void draw() = 0;
	//virtual void drawCPU();
};

/*
some hazards aren't bad

list of (eventual) hazards:
stationary turret
turret
patrolling turret
lightning (abstract class: generalized lightning, has point to point lightning bolt)
vertical lightning
lightning square (really a rectangle)
lightning circle
lava
no bullet zone
teleport zone



list of possible hazards, joke or not:
the reflecktor (bounces/reflects tanks when they hit it)


things the hazards need to access, override, and/or deal with (of bullets and tanks, and hazards?):
...all good? probably just need x and y (and r)





*/