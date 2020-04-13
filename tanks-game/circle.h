#pragma once

class Circle {
	friend class DeveloperManager;
	friend class CollisionHandler;
	friend class PowerFunctionHelper;

public:
	double x;
	double y;
	double r;
	
public:
	static const unsigned int numOfSides = 64;
};