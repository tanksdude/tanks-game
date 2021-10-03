#pragma once
#include "colorvalueholder.h"

class GeneralizedNoBulletZone {
	//the sole purpose of this is to provide the color
protected:
	virtual ColorValueHolder getColor() const;
	virtual void tick(); //does nothing...
};
