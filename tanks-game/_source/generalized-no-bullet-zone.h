#pragma once
#include "color-value-holder.h"

class GeneralizedNoBulletZone {
	//provides almost nothing
public:
	static const float X_WIDTH;
	static const ColorValueHolder X_COLOR;
public:
	virtual ColorValueHolder getColor() const;
	//virtual void tick() { return; } //does nothing...
};
