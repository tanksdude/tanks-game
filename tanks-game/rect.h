#pragma once

struct Rect {
	friend class DeveloperManager;
	friend class CollisionHandler;
	friend class PowerFunctionHelper;

public:
	double x;
	double y;
	double w;
	double h;

	inline double getX() const { return x; }
	inline double getY() const { return y; }
	inline double getW() const { return w; }
	inline double getH() const { return h; }
};
