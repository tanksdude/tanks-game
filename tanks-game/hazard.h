#pragma once
#include "gamething.h"
#include "colorvalueholder.h"

class Hazard : public GameThing {
	friend class ResetThings;
protected:
	bool canAcceptPowers;

public:
	//virtual bool getCanAcceptPowers();
	virtual bool validLocation() { return true; } //false if unable to operate properly where it is; "operate properly" is a little vague
	virtual bool reasonableLocation() = 0;

	virtual void tick() = 0;
};
