#pragma once
#include "circle.h"

class Point : public Circle {
public:
	Point(double x, double y) {
		this->x = x;
		this->y = y;
		this->r = 0;
	}
};
