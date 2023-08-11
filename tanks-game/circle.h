#pragma once

struct Circle {
	friend class DeveloperManager;
	friend class CollisionHandler;
	friend class PowerFunctionHelper;

public:
	static const unsigned int numOfSides = 64;

public:
	double x;
	double y;
	double r;

	inline double getX() const { return x; }
	inline double getY() const { return y; }
	inline double getR() const { return r; }
};
