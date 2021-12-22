#pragma once
#include "tank.h"

class ResetThings {
public:
	static const double default_tankToEdgeDist;

public:
	static void reset() { ResetThings::reset(0); }
	static void reset(int);
	static void firstReset(); //temporary; just for main() to do its thing while I'm still testing
	static void tankPositionReset(Tank* first, Tank* second); //use this one
	static void tankPositionReset(Tank* first, Tank* second, double x); //distance from edge
	static void tankPositionReset(Tank* first, Tank* second, double x, double y); //x and y are first's position; second is opposite first

private:
	ResetThings() {}
	ResetThings(const ResetThings&) {}
};
