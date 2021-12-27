#pragma once
#include <vector>
#include <utility>

#include "circle.h"
#include "rect.h"

class PhysicsHandler {
protected:
	struct ObjectIntervalInfo {
		double xStart;
		double xEnd;
		int listIndex;
		bool collider;
		ObjectIntervalInfo() : ObjectIntervalInfo(0, 0, 0, false) {}
		ObjectIntervalInfo(const Rect* r, int i, bool col) : ObjectIntervalInfo(r->getX(), r->getX() + r->getW(), i, col) {}
		ObjectIntervalInfo(const Circle* c, int i, bool col) : ObjectIntervalInfo(c->getX() - c->getR(), c->getX() + c->getR(), i, col) {}
		ObjectIntervalInfo(double x1, double x2, int i, bool c) {
			xStart = x1;
			xEnd = x2;
			listIndex = i;
			collider = c;
		}
	};

public:
	//template<typename T, typename U>
	//static std::vector<std::pair<T, U>> sweepAndPrune(const std::vector<T>& collider, const std::vector<U>& collidee);
	template<typename T, typename U>
	static std::vector<std::pair<int, int>> sweepAndPrune(const std::vector<T>& collider, const std::vector<U>& collidee);
	template<typename T>
	static std::vector<std::pair<int, int>> sweepAndPrune(const std::vector<T>& collider);

private:
	PhysicsHandler() {}
	PhysicsHandler(const PhysicsHandler&) {}
};
