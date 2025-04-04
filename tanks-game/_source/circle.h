#pragma once

struct Circle {
	friend class DeveloperManager;
	friend class CollisionHandler;
	friend class PowerFunctionHelper;

public:
	static constexpr unsigned int NumOfSides = 64;

public:
	double x;
	double y;
	double r;

	double getX() const noexcept { return x; }
	double getY() const noexcept { return y; }
	double getR() const noexcept { return r; }
};

static_assert(Circle::NumOfSides % 2 == 0);
