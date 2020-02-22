#pragma once
#include "hazard.h"
#include "circlehazard.h"
#include "recthazard.h"
#include <vector>

class HazardManager {
	friend class ResetThings;
private:
	static std::vector<CircleHazard*> circleHazards;
	static std::vector<RectHazard*> rectHazards;
public:
	static void initialize();
	static CircleHazard* const getCircleHazard(int index);
	static RectHazard* const getRectHazard(int index);
	//see BulletManager explanation
	static void pushCircleHazard(CircleHazard*);
	static void pushRectHazard(RectHazard*);
	static int getNumCircleHazards() { return circleHazards.size(); }
	static int getNumRectHazards() { return rectHazards.size(); }
	static void deleteCircleHazard(int index);
	static void deleteRectHazard(int index);
};