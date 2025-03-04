#include "physics-handler.h"

#include <algorithm>
#include <iostream>

#include "diagnostics.h"

template<typename T, typename U>
std::vector<std::pair<int, int>>* PhysicsHandler::sweepAndPrune(const std::vector<T>& collider, const std::vector<U>& collidee) {
	std::vector<std::pair<int, int>>* collisionList = new std::vector<std::pair<int, int>>;

	//find object intervals
	std::vector<ObjectIntervalInfo> objectIntervals; objectIntervals.reserve(collider.size() + collidee.size());
	for (int i = 0; i < collider.size(); i++) {
		T o = collider[i];
		objectIntervals.push_back(ObjectIntervalInfo(o, i, true));
	}
	for (int j = 0; j < collidee.size(); j++) {
		U o = collidee[j];
		objectIntervals.push_back(ObjectIntervalInfo(o, j, false));
	}
	std::sort(objectIntervals.begin(), objectIntervals.end(),
		[](const ObjectIntervalInfo& lhs, const ObjectIntervalInfo& rhs) { return (lhs.xStart < rhs.xStart); });

	//sweep through
	std::vector<ObjectIntervalInfo> iteratingObjects; iteratingObjects.reserve(objectIntervals.size());
	#if _DEBUG
	//performance seems about the same with or without reserving
	#else
	collisionList->reserve(collider.size()*collidee.size());
	#endif
	for (int i = 0; i < objectIntervals.size(); i++) {
		const ObjectIntervalInfo& currentObject = objectIntervals[i];
		for (int j = 0; j < iteratingObjects.size(); j++) {
			//remove if not in active interval
			if (iteratingObjects[j].xEnd < currentObject.xStart) {
				iteratingObjects.erase(iteratingObjects.begin() + j);
				j--;
				continue;
			}
			//push possible collision
			if (iteratingObjects[j].collider && !currentObject.collider) {
				collisionList->push_back(std::pair<int, int>(iteratingObjects[j].listIndex, currentObject.listIndex));
			} else if (currentObject.collider && !iteratingObjects[j].collider) {
				collisionList->push_back(std::pair<int, int>(currentObject.listIndex, iteratingObjects[j].listIndex));
			}
		}

		iteratingObjects.push_back(currentObject);
	}

	return collisionList;
}

template std::vector<std::pair<int, int>>* PhysicsHandler::sweepAndPrune<Rect*, Rect*>
(const std::vector<Rect*>& collider, const std::vector<Rect*>& collidee);
template std::vector<std::pair<int, int>>* PhysicsHandler::sweepAndPrune<Rect*, Circle*>
(const std::vector<Rect*>& collider, const std::vector<Circle*>& collidee);
template std::vector<std::pair<int, int>>* PhysicsHandler::sweepAndPrune<Circle*, Rect*>
(const std::vector<Circle*>& collider, const std::vector<Rect*>& collidee);
template std::vector<std::pair<int, int>>* PhysicsHandler::sweepAndPrune<Circle*, Circle*>
(const std::vector<Circle*>& collider, const std::vector<Circle*>& collidee);

template<typename T>
std::vector<std::pair<int, int>>* PhysicsHandler::sweepAndPrune(const std::vector<T>& collider) {
	//pretty sure that can be multithreaded, as long as there's a mutex when pushing to the list; also, should do chunking since bullets with nearby indices probably have similar locations (spatial locality, kinda)

	std::vector<std::pair<int, int>>* collisionList = new std::vector<std::pair<int, int>>;

	//find object intervals
	//TODO: so this step isn't supposed to be computed every time; information should be stored, then insertion sort can be used to fix the new data
	//auto start = Diagnostics::getTime();
	std::vector<ObjectIntervalInfo> objectIntervals; objectIntervals.reserve(collider.size());
	for (int i = 0; i < collider.size(); i++) {
		T o = collider[i];
		objectIntervals.push_back(ObjectIntervalInfo(o, i, true));
	}
	std::sort(objectIntervals.begin(), objectIntervals.end(),
		[](const ObjectIntervalInfo& lhs, const ObjectIntervalInfo& rhs) { return (lhs.xStart < rhs.xStart); });
	//auto end = Diagnostics::getTime();
	//std::cout << "intervals: " << (long double)Diagnostics::getDiff(start, end) << "ms" << std::endl;

	//sweep through
	//BIG TODO: is it possible to multithread this? because it's the biggest timesink (maybe the inner loop can be parallelized?)
	//start = Diagnostics::getTime();
	std::vector<ObjectIntervalInfo> iteratingObjects; iteratingObjects.reserve(objectIntervals.size());
	#if _DEBUG
	//performance seems about the same with or without reserving
	#else
	collisionList->reserve(objectIntervals.size()*objectIntervals.size());
	#endif
	for (int i = 0; i < objectIntervals.size(); i++) {
		const ObjectIntervalInfo& currentObject = objectIntervals[i];
		for (int j = 0; j < iteratingObjects.size(); j++) {
			//prune if not in active interval
			if (iteratingObjects[j].xEnd < currentObject.xStart) {
				iteratingObjects.erase(iteratingObjects.begin() + j);
				j--;
				continue;
			}
			//push possible collision
			collisionList->push_back(std::pair<int, int>(currentObject.listIndex, iteratingObjects[j].listIndex));
		}

		iteratingObjects.push_back(currentObject);
	}
	//end = Diagnostics::getTime();
	//std::cout << "sweep: " << (long double)Diagnostics::getDiff(start, end) << "ms" << std::endl << std::endl;

	return collisionList;
	//TODO: this algorithm is somewhat simple; how difficult would it be to do it on the GPU?
}

template std::vector<std::pair<int, int>>* PhysicsHandler::sweepAndPrune<Rect*>
(const std::vector<Rect*>& collider);
template std::vector<std::pair<int, int>>* PhysicsHandler::sweepAndPrune<Circle*>
(const std::vector<Circle*>& collider);



/*
template<typename T>
static std::unordered_map<std::pair<int, int>, std::vector<T>>* PhysicsHandler::bucketPlacement(const std::vector<T>& collider, double leftBound, double rightBound, double downBound, double upBound, int lrBuckets, int udBuckets) {
	auto* collisionList = new std::unordered_map<std::pair<int, int>, std::vector<T>>;

	for (int i = 0; i < collider.size(); i++) {
		T o = collider[i];
		auto bounds = ObjectIntervalInfo(o, i, true);

		int lr1 = int((bounds.xStart - leftBound) / (rightBound - leftBound) * lrBuckets);
		int lr2 = int((bounds.xEnd - leftBound) / (rightBound - leftBound) * lrBuckets);
		int ud1 = int((bounds.yStart - downBound) / (upBound - downBound) * udBuckets);
		int ud2 = int((bounds.yEnd - downBound) / (upBound - downBound) * udBuckets);

		for (int i = lr1; i <= lr2; i++) {
			for (int j = ud1; j <= ud2; j++) {
				if (collisionList->find({ i,j }) == collisionList->end()) {
					collisionList->insert({ {i, j}, std::vector<T>{o} });
				} else {
					collisionList->at( {i, j} ).push_back(o);
				}
			}
		}
	}

	return collisionList;
}

template std::unordered_map<std::pair<int, int>, std::vector<Rect*>>* PhysicsHandler::bucketPlacement<Rect*>
(const std::vector<Rect*>& collider, double leftBound, double rightBound, double downBound, double upBound, int lBuckets, int rBuckets);
template std::unordered_map<std::pair<int, int>, std::vector<Circle*>>* PhysicsHandler::bucketPlacement<Circle*>
(const std::vector<Circle*>& collider, double leftBound, double rightBound, double downBound, double upBound, int lBuckets, int rBuckets);
*/
