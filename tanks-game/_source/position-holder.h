#pragma once

struct PositionHolder {
	double x;
	double y;
	PositionHolder(double x_, double y_) {
		x = x_;
		y = y_;
	}
	PositionHolder() : PositionHolder(0, 0) {}
};
//maybe use std::pair?
