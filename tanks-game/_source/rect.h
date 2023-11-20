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

	double getX() const noexcept { return x; }
	double getY() const noexcept { return y; }
	double getW() const noexcept { return w; }
	double getH() const noexcept { return h; }
};
