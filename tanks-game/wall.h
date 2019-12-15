#pragma once
#include "rect.h"
#include "colorvalueholder.h"
#include "resetthings.h"

class Wall : public Rect{
	friend class ResetThings; //not really needed
	ColorValueHolder color;
	short id = -1;

public:
	Wall(double x, double y, double w, double h, ColorValueHolder);
	Wall(double x, double y, double w, double h, ColorValueHolder c, short id);

	short getID() {
		return id;
	}
	void draw();
};