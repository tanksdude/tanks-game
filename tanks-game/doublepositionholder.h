#pragma once

//no, this is not a struct that holds two PositionHolder
struct DoublePositionHolder {
	double x1;
	double x2;
	double y1;
	double y2;
	DoublePositionHolder(double x1, double x2, double y1, double y2) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}
};
