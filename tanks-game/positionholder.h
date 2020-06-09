#pragma once

struct PositionHolder {
	double x;
	double y;
	PositionHolder() {
		x = y = 0;
	}
	PositionHolder(double x_, double y_) {
		x = x_;
		y = y_;
	}
};
