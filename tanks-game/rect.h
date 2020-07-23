#pragma once

class Rect {
	friend class DeveloperManager;
	friend class CollisionHandler;
	friend class PowerFunctionHelper;

public:
	double x;
	double y;
	double w;
	double h;

	double getX() const { return x; }
	double getY() const { return y; }
	double getW() const { return w; }
	double getH() const { return h; }
};
