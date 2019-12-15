#pragma once
#include "rect.h"

class WallEdge : public Rect {
public:
	WallEdge(double x_, double y_, double w_, double h_);
	void draw();
};
