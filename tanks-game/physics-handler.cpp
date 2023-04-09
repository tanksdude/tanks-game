#include "physics-handler.h"
#include <algorithm>

/*
template<typename T, typename U>
std::vector<std::pair<T, U>> PhysicsHandler::sweepAndPrune(const std::vector<T>& collider, const std::vector<U>& collidee) {
	std::vector<std::pair<T, U>> collisionList;

	//TODO

	return collisionList;
}

//order from GameMainLoop
template std::vector<std::pair<Tank*, PowerSquare*>> PhysicsHandler::sweepAndPrune<Tank*, PowerSquare*>
(const std::vector<Tank*>& collider, const std::vector<PowerSquare*>& collidee);

template std::vector<std::pair<Tank*, Wall*>> PhysicsHandler::sweepAndPrune<Tank*, Wall*>
(const std::vector<Tank*>& collider, const std::vector<Wall*>& collidee);

template std::vector<std::pair<Tank*, CircleHazard*>> PhysicsHandler::sweepAndPrune<Tank*, CircleHazard*>
(const std::vector<Tank*>& collider, const std::vector<CircleHazard*>& collidee);
template std::vector<std::pair<Tank*, RectHazard*>> PhysicsHandler::sweepAndPrune<Tank*, RectHazard*>
(const std::vector<Tank*>& collider, const std::vector<RectHazard*>& collidee);

template std::vector<std::pair<Tank*, Tank*>> PhysicsHandler::sweepAndPrune<Tank*, Tank*>
(const std::vector<Tank*>& collider, const std::vector<Tank*>& collidee);

template std::vector<std::pair<Bullet*, Wall*>> PhysicsHandler::sweepAndPrune<Bullet*, Wall*>
(const std::vector<Bullet*>& collider, const std::vector<Wall*>& collidee);

template std::vector<std::pair<Bullet*, CircleHazard*>> PhysicsHandler::sweepAndPrune<Bullet*, CircleHazard*>
(const std::vector<Bullet*>& collider, const std::vector<CircleHazard*>& collidee);
template std::vector<std::pair<Bullet*, RectHazard*>> PhysicsHandler::sweepAndPrune<Bullet*, RectHazard*>
(const std::vector<Bullet*>& collider, const std::vector<RectHazard*>& collidee);

template std::vector<std::pair<Bullet*, Bullet*>> PhysicsHandler::sweepAndPrune<Bullet*, Bullet*>
(const std::vector<Bullet*>& collider, const std::vector<Bullet*>& collidee);

template std::vector<std::pair<Bullet*, Tank*>> PhysicsHandler::sweepAndPrune<Bullet*, Tank*>
(const std::vector<Bullet*>& collider, const std::vector<Tank*>& collidee);
//end order
*/

template<typename T, typename U>
std::vector<std::pair<int, int>> PhysicsHandler::sweepAndPrune(const std::vector<T>& collider, const std::vector<U>& collidee) {
	std::vector<std::pair<int, int>> collisionList;

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
		[](const ObjectIntervalInfo& lhs, const ObjectIntervalInfo& rhs) { return (lhs.xStart < rhs.xStart); }); //lambda

	//sweep through
	std::vector<ObjectIntervalInfo> iteratingObjects;
	for (int i = 0; i < objectIntervals.size(); i++) {
		ObjectIntervalInfo currentObject = objectIntervals[i];
		for (int j = 0; j < iteratingObjects.size(); j++) {
			//remove if not in active interval
			if (iteratingObjects[j].xEnd < currentObject.xStart) {
				iteratingObjects.erase(iteratingObjects.begin() + j);
				j--;
				continue;
			}
			//push possible collision
			if (iteratingObjects[j].collider && !currentObject.collider) {
				collisionList.push_back(std::pair<int, int>(iteratingObjects[j].listIndex, currentObject.listIndex));
			} else if (currentObject.collider && !iteratingObjects[j].collider) {
				collisionList.push_back(std::pair<int, int>(currentObject.listIndex, iteratingObjects[j].listIndex));
			}
		}

		iteratingObjects.push_back(currentObject);
	}

	return collisionList;
}

template std::vector<std::pair<int, int>> PhysicsHandler::sweepAndPrune<Rect*, Rect*>
(const std::vector<Rect*>& collider, const std::vector<Rect*>& collidee);
template std::vector<std::pair<int, int>> PhysicsHandler::sweepAndPrune<Rect*, Circle*>
(const std::vector<Rect*>& collider, const std::vector<Circle*>& collidee);
template std::vector<std::pair<int, int>> PhysicsHandler::sweepAndPrune<Circle*, Rect*>
(const std::vector<Circle*>& collider, const std::vector<Rect*>& collidee);
template std::vector<std::pair<int, int>> PhysicsHandler::sweepAndPrune<Circle*, Circle*>
(const std::vector<Circle*>& collider, const std::vector<Circle*>& collidee);

template<typename T>
std::vector<std::pair<int, int>> PhysicsHandler::sweepAndPrune(const std::vector<T>& collider) {
	std::vector<std::pair<int, int>> collisionList;

	//find object intervals
	//TODO: this step isn't supposed to be computed every time; information should be stored, then insertion sort can be used to fix the new data
	std::vector<ObjectIntervalInfo> objectIntervals; objectIntervals.reserve(collider.size());
	for (int i = 0; i < collider.size(); i++) {
		T o = collider[i];
		objectIntervals.push_back(ObjectIntervalInfo(o, i, true));
	}
	std::sort(objectIntervals.begin(), objectIntervals.end(),
		[](const ObjectIntervalInfo& lhs, const ObjectIntervalInfo& rhs) { return (lhs.xStart < rhs.xStart); }); //lambda

	//sweep through
	//BIG TODO: is it possible to multithread this? because it's the biggest timesink
	std::vector<ObjectIntervalInfo> iteratingObjects;
	for (int i = 0; i < objectIntervals.size(); i++) {
		ObjectIntervalInfo currentObject = objectIntervals[i];
		for (int j = 0; j < iteratingObjects.size(); j++) {
			//prune if not in active interval
			if (iteratingObjects[j].xEnd < currentObject.xStart) {
				iteratingObjects.erase(iteratingObjects.begin() + j);
				j--;
				continue;
			}
			//push possible collision
			collisionList.push_back(std::pair<int, int>(currentObject.listIndex, iteratingObjects[j].listIndex));
		}

		iteratingObjects.push_back(currentObject);
	}

	return collisionList;
	//TODO: this algorithm is somewhat simple; how difficult would it be to do it on the GPU?
}

template std::vector<std::pair<int, int>> PhysicsHandler::sweepAndPrune<Rect*>
(const std::vector<Rect*>& collider);
template std::vector<std::pair<int, int>> PhysicsHandler::sweepAndPrune<Circle*>
(const std::vector<Circle*>& collider);
