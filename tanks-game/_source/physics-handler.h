#pragma once
#include <utility>
#include <vector>
#include <unordered_map>

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
	template<typename T, typename U>
	static std::vector<std::pair<int, int>>* sweepAndPrune(const std::vector<T>& collider, const std::vector<U>& collidee);
	template<typename T>
	static std::vector<std::pair<int, int>>* sweepAndPrune(const std::vector<T>& collider);

	//template<typename T>
	//static std::unordered_map<std::pair<int, int>, std::vector<T>>* bucketPlacement(const std::vector<T>& collider, double leftBound, double rightBound, double downBound, double upBound, int lrBuckets, int udBuckets);
	////since bucket sort is a real thing this shouldn't call be called that https://en.wikipedia.org/wiki/Bucket_sort
	//problem: can't do the bucket thing (spatial partitioning is its official name, I think) because individual elements can't be larger than a cell
	//solution: quadtrees (I put this off for so long)
	//idea: quadtree fast path for small stuff, sweep and prune backup for larger stuff

	//template<typename T> /* TODO return type; probably a new class */
	//static std::unordered_map<std::pair<int, int>, std::vector<T>>* generateQuadtree(const std::vector<T>& collider, double leftBound, double rightBound, double downBound, double upBound, int lrBuckets, int udBuckets);
	////https://stackoverflow.com/a/48355534
	////https://stackoverflow.com/a/48384354

private:
	PhysicsHandler() = delete;
	PhysicsHandler(const PhysicsHandler&) = delete;
};
