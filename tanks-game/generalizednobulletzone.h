#pragma once
#include "colorvalueholder.h"

class GeneralizedNoBulletZone {
	//the sole purpose of this is to provide the color
public:
	virtual ColorValueHolder getColor() const;
	virtual void tick(); //does nothing...
};
