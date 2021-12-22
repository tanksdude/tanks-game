#pragma once
#include "tank.h"
#include <vector>
#include "cannon-point.h"
#include "bullet.h"
#include "wall.h"

class PowerFunctionHelper {
protected:
	static inline bool bounceGenericCornerHandler(Bullet* b, Wall* w, double x, double y) {
		return superbounceGenericCornerHandler(b, w, x, y, 0);
	}
	static bool superbounceGenericCornerHandler(Bullet*, Wall*, double x, double y, double strength = 0);

public:
	static Game_ID homingGenericTarget(const Bullet*, bool targetUsingAngleDiff); //find target for homing
	static void homingGenericMove(Bullet*, Game_ID targetID, double maxAngleChange); //do homing on target

	static inline bool bounceGeneric(Bullet* b, Wall* w) { return superbounceGeneric(b, w, 0); }
	static inline bool bounceGenericWithCorners(Bullet* b, Wall* w) { return superbounceGenericWithCorners(b, w, 0); }
	static inline bool bounceEdgeGenericX(Bullet* b) { return superbounceEdgeGenericX(b, 0); }
	static inline bool bounceEdgeGenericY(Bullet* b) { return superbounceEdgeGenericY(b, 0); }
	//superbounce: bounce except push the wall (default parameter to make sure it's used intentionally)
	static bool superbounceGeneric(Bullet*, Wall*, double strength = 0);
	static bool superbounceGenericWithCorners(Bullet*, Wall*, double strength = 0);
	static bool superbounceEdgeGenericX(Bullet* b, double strength = 0); //strength not needed since an edge can't be pushed
	static bool superbounceEdgeGenericY(Bullet* b, double strength = 0);

	static void equallySpacedCannonPoints(Tank*, std::vector<CannonPoint>*, int); //fix?

private:
	PowerFunctionHelper() {}
	PowerFunctionHelper(const PowerFunctionHelper&) {}
};
