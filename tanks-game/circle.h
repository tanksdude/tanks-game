#pragma once

class Circle {
	friend class DeveloperManager;
	friend class CollisionHandler;
	friend class PowerFunctionHelper;

public:
	static const unsigned int numOfSides = 64;

public:
	double x;
	double y;
	double r;

	double getX() const { return x; }
	double getY() const { return y; }
	double getR() const { return r; }
};
