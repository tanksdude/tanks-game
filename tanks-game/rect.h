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
};