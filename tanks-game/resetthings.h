#pragma once
#include "tank.h"

class ResetThings {
public:
	static void reset() { ResetThings::reset(0); }
	static void reset(int);
	static void firstReset(); //temporary; just for main() to do its thing while I'm still testing
	static void tankPositionReset(Tank* first, Tank* second, int randNum); //use this one (randNum: [0,4])
	static void tankPositionReset(Tank* first, Tank* second, double x, int randNum); //only used by bigfunlevel right now
	static void tankPositionReset(Tank* first, Tank* second, double x, double y, bool serious); //x and y are first's position; second is opposite first (serious only differentiates it from the above function)

private:
	ResetThings() {}
	ResetThings(const ResetThings&) {}
};
