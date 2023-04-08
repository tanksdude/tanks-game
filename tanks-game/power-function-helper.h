#pragma once
#include "tank.h"
#include <vector>
#include "cannon-point.h"
#include "bullet.h"
#include "wall.h"
#include <utility>

class PowerFunctionHelper {
protected:
	static inline std::pair<bool, InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct>> bounceGenericCornerHandler(const Bullet* b, const Wall* w, double x, double y) {
		return superbounceGenericCornerHandler(b, w, x, y, 0);
	}
	static std::pair<bool, InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct>> superbounceGenericCornerHandler(const Bullet*, const Wall*, double x, double y, double strength = 0);

public:
	static Game_ID homingGenericTarget(const Bullet*, bool targetUsingAngleDiff); //find target for homing
	static void homingGenericMove(Bullet*, Game_ID targetID, double maxAngleChange); //do homing on target

	static inline std::pair<bool, InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct>> bounceGeneric(const Bullet* b, const Wall* w) { return superbounceGeneric(b, w, 0); }
	static inline std::pair<bool, InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct>> bounceGenericWithCorners(const Bullet* b, const Wall* w) { return superbounceGenericWithCorners(b, w, 0); }
	static inline std::pair<bool, BulletUpdateStruct> bounceEdgeGenericX(const Bullet* b) { return superbounceEdgeGenericX(b, 0); }
	static inline std::pair<bool, BulletUpdateStruct> bounceEdgeGenericY(const Bullet* b) { return superbounceEdgeGenericY(b, 0); }
	//superbounce: bounce except push the wall (default parameter to make sure it's used intentionally)
	static std::pair<bool, InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct>> superbounceGeneric(const Bullet*, const Wall*, double strength = 0);
	static std::pair<bool, InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct>> superbounceGenericWithCorners(const Bullet*, const Wall*, double strength = 0);
	static std::pair<bool, BulletUpdateStruct> superbounceEdgeGenericX(const Bullet* b, double strength = 0); //strength not needed since an edge can't be pushed
	static std::pair<bool, BulletUpdateStruct> superbounceEdgeGenericY(const Bullet* b, double strength = 0);

	static void equallySpacedCannonPoints(Tank*, std::vector<CannonPoint>*, int); //fix?

private:
	PowerFunctionHelper() {}
	PowerFunctionHelper(const PowerFunctionHelper&) {}
};
