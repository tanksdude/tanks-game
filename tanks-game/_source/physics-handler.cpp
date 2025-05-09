#include "physics-handler.h"

#include <algorithm>
#include <iostream>
#include <execution>

#include "diagnostics.h"

#include "aaa_first.h"

uint32_t PhysicsHandler::MinTaskSize;
PhysicsHandler::SweepAndPruneTask* PhysicsHandler::s_physicsTask;
PhysicsHandler::SweepAndPruneTask_TwoLists* PhysicsHandler::s_physicsTask_TwoLists;

void PhysicsHandler::Initialize(uint32_t MinTaskSize_) {
	MinTaskSize = MinTaskSize_;

	s_physicsTask = new SweepAndPruneTask(g_TS.GetNumTaskThreads());
	s_physicsTask_TwoLists = new SweepAndPruneTask_TwoLists(g_TS.GetNumTaskThreads());
}

void PhysicsHandler::Uninitialize() {
	delete s_physicsTask;
	delete s_physicsTask_TwoLists;
}

PhysicsHandler::SweepAndPruneTask_TwoLists::SweepAndPruneTask_TwoLists(int num_threads_) {
	(void) m_objectIntervals; (void) m_SetSize;
	num_threads = num_threads_;
	m_MinRange = PhysicsHandler::MinTaskSize;
	m_collisionLists = new std::vector<std::pair<int, int>>*[num_threads_];
	for (int i = 0; i < num_threads_; i++) {
		m_collisionLists[i] = new std::vector<std::pair<int, int>>;
		#if _DEBUG
		//performance seems about the same with or without reserving
		#else
		m_collisionLists[i]->reserve(PhysicsHandler::MinTaskSize * PhysicsHandler::MinTaskSize / 2); //half just to save memory in the very likely scenario everything in range is not touching everything in range
		#endif
	}
}

void PhysicsHandler::SweepAndPruneTask_TwoLists::Init(const std::vector<PhysicsHandler::ObjectIntervalInfo>* objectIntervals, int task_size) {
	m_objectIntervals = objectIntervals;
	m_SetSize = task_size;
}

PhysicsHandler::SweepAndPruneTask_TwoLists::~SweepAndPruneTask_TwoLists() {
	for (int i = 0; i < num_threads; i++) {
		delete m_collisionLists[i];
	}
	delete[] m_collisionLists;
}

void PhysicsHandler::SweepAndPruneTask_TwoLists::ExecuteRange(enki::TaskSetPartition range_, uint32_t threadnum_) {
	//TODO: this is the largest timesink, somehow; maybe there's a way to avoid all the new/delete due to constantly constructing/destructing stuff (though that's probably not the main issue)
	const double finalXStart = m_objectIntervals->data()[range_.end - 1].xStart;
	std::vector<PhysicsHandler::ObjectIntervalInfo> iteratingObjects; iteratingObjects.reserve(range_.end - range_.start); //possible a resize will be needed, it's okay
	for (int i = range_.start; i < m_objectIntervals->size(); i++) {
		//NOTE: this goes past the range end because there are objects on the boundary; if it goes to the range end, collision pairs will be missed

		const PhysicsHandler::ObjectIntervalInfo& currentObject = m_objectIntervals->data()[i];
		bool everyObjectIsOutOfRange = true;
		for (int j = 0; j < iteratingObjects.size(); j++) {
			//prune if not in active interval
			if (iteratingObjects[j].xEnd < currentObject.xStart) {
				iteratingObjects.erase(iteratingObjects.begin() + j);
				j--;
				continue;
			}
			//push possible collision
			if (iteratingObjects[j].xStart <= finalXStart) [[likely]] {
				//TODO: what happens if two objects have the exact same xStart?
				if (iteratingObjects[j].collider && !currentObject.collider) {
					m_collisionLists[threadnum_]->push_back(std::pair<int, int>(iteratingObjects[j].listIndex, currentObject.listIndex));
				} else if (currentObject.collider && !iteratingObjects[j].collider) {
					m_collisionLists[threadnum_]->push_back(std::pair<int, int>(currentObject.listIndex, iteratingObjects[j].listIndex));
				}
				everyObjectIsOutOfRange = false;
			}
		}
		if (i >= range_.end && everyObjectIsOutOfRange) [[unlikely]] {
			return;
		}

		iteratingObjects.push_back(currentObject);
	}
}

PhysicsHandler::SweepAndPruneTaskGroup_TwoLists::SweepAndPruneTaskGroup_TwoLists(PhysicsHandler::SweepAndPruneTask_TwoLists* testTask_ ) : m_Dependency(testTask_, this) {
	m_testTasks = testTask_;
	final_collisionList = new std::vector<std::pair<int, int>>;
}

PhysicsHandler::SweepAndPruneTaskGroup_TwoLists::~SweepAndPruneTaskGroup_TwoLists() {
	//don't delete final_collisionList
}

void PhysicsHandler::SweepAndPruneTaskGroup_TwoLists::ExecuteRange(enki::TaskSetPartition range, uint32_t threadnum) {
	size_t size = 0;
	for (int i = 0; i < m_testTasks->num_threads; i++) {
		size += m_testTasks->m_collisionLists[i]->size();
	}

	final_collisionList->reserve(size);
	for (int i = 0; i < m_testTasks->num_threads; i++) {
		final_collisionList->insert(final_collisionList->end(), m_testTasks->m_collisionLists[i]->begin(), m_testTasks->m_collisionLists[i]->end());
		m_testTasks->m_collisionLists[i]->clear();
	}
}

PhysicsHandler::SweepAndPruneTask::SweepAndPruneTask(int num_threads_) {
	(void) m_objectIntervals; (void) m_SetSize;
	num_threads = num_threads_;
	m_MinRange = PhysicsHandler::MinTaskSize;
	m_collisionLists = new std::vector<std::pair<int, int>>*[num_threads_];
	for (int i = 0; i < num_threads_; i++) {
		m_collisionLists[i] = new std::vector<std::pair<int, int>>;
		#if _DEBUG
		//performance seems about the same with or without reserving
		#else
		m_collisionLists[i]->reserve(PhysicsHandler::MinTaskSize * PhysicsHandler::MinTaskSize / 2); //half just to save memory in the very likely scenario everything in range is not touching everything in range
		#endif
	}
}

void PhysicsHandler::SweepAndPruneTask::Init(const std::vector<PhysicsHandler::ObjectIntervalInfo>* objectIntervals, int task_size) {
	m_objectIntervals = objectIntervals;
	m_SetSize = task_size;
}

PhysicsHandler::SweepAndPruneTask::~SweepAndPruneTask() {
	for (int i = 0; i < num_threads; i++) {
		delete m_collisionLists[i];
	}
	delete[] m_collisionLists;
}

void PhysicsHandler::SweepAndPruneTask::ExecuteRange(enki::TaskSetPartition range_, uint32_t threadnum_) {
	const double finalXStart = m_objectIntervals->data()[range_.end - 1].xStart;
	std::vector<PhysicsHandler::ObjectIntervalInfo> iteratingObjects; iteratingObjects.reserve(range_.end - range_.start); //possible a resize will be needed, it's okay
	for (int i = range_.start; i < m_objectIntervals->size(); i++) {
		//NOTE: this goes past the range end because there are objects on the boundary; if it goes to the range end, collision pairs will be missed

		const PhysicsHandler::ObjectIntervalInfo& currentObject = m_objectIntervals->data()[i];
		bool everyObjectIsOutOfRange = true;
		for (int j = 0; j < iteratingObjects.size(); j++) {
			//prune if not in active interval
			if (iteratingObjects[j].xEnd < currentObject.xStart) {
				iteratingObjects.erase(iteratingObjects.begin() + j);
				j--;
				continue;
			}
			//push possible collision
			if (iteratingObjects[j].xStart <= finalXStart) [[likely]] {
				//TODO: what happens if two objects have the exact same xStart?
				m_collisionLists[threadnum_]->push_back(std::pair<int, int>(currentObject.listIndex, iteratingObjects[j].listIndex));
				everyObjectIsOutOfRange = false;
			}
		}
		if (i >= range_.end && everyObjectIsOutOfRange) [[unlikely]] {
			return;
		}

		iteratingObjects.push_back(currentObject);
	}
}

PhysicsHandler::SweepAndPruneTaskGroup::SweepAndPruneTaskGroup(PhysicsHandler::SweepAndPruneTask* testTask_ ) : m_Dependency(testTask_, this) {
	m_testTasks = testTask_;
	final_collisionList = new std::vector<std::pair<int, int>>;
}

PhysicsHandler::SweepAndPruneTaskGroup::~SweepAndPruneTaskGroup() {
	//don't delete final_collisionList
}

void PhysicsHandler::SweepAndPruneTaskGroup::ExecuteRange(enki::TaskSetPartition range, uint32_t threadnum) {
	size_t size = 0;
	for (int i = 0; i < m_testTasks->num_threads; i++) {
		size += m_testTasks->m_collisionLists[i]->size();
	}

	final_collisionList->reserve(size);
	for (int i = 0; i < m_testTasks->num_threads; i++) {
		final_collisionList->insert(final_collisionList->end(), m_testTasks->m_collisionLists[i]->begin(), m_testTasks->m_collisionLists[i]->end());
		m_testTasks->m_collisionLists[i]->clear();
	}
}



template<typename T, typename U>
std::vector<std::pair<int, int>>* PhysicsHandler::sweepAndPrune(const std::vector<T>& collider, const std::vector<U>& collidee) {
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
	std::sort(std::execution::par, objectIntervals.begin(), objectIntervals.end(),
		[](const ObjectIntervalInfo& lhs, const ObjectIntervalInfo& rhs) { return (lhs.xStart < rhs.xStart); });

	//sweep through
	s_physicsTask_TwoLists->Init(&objectIntervals, collider.size() + collidee.size());
	SweepAndPruneTaskGroup_TwoLists physicsTaskGroup(s_physicsTask_TwoLists);

	g_TS.AddTaskSetToPipe(s_physicsTask_TwoLists);
	g_TS.WaitforTask(&physicsTaskGroup);

	return physicsTaskGroup.final_collisionList;
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
	//find object intervals
	//TODO: so this step isn't supposed to be computed every time; information should be stored, then insertion sort can be used to fix the new data
	//auto start = Diagnostics::getTime();
	std::vector<ObjectIntervalInfo> objectIntervals; objectIntervals.reserve(collider.size());
	for (int i = 0; i < collider.size(); i++) {
		T o = collider[i];
		objectIntervals.push_back(ObjectIntervalInfo(o, i, true));
	}
	//TODO: try parallel sorting (using enkiTS)
	std::sort(std::execution::par, objectIntervals.begin(), objectIntervals.end(),
		[](const ObjectIntervalInfo& lhs, const ObjectIntervalInfo& rhs) { return (lhs.xStart < rhs.xStart); });
	//auto end = Diagnostics::getTime();
	//std::cout << "intervals: " << (long double)Diagnostics::getDiff(start, end) << "ms" << std::endl;

	//sweep through
	//start = Diagnostics::getTime();
	s_physicsTask->Init(&objectIntervals, collider.size());
	SweepAndPruneTaskGroup physicsTaskGroup(s_physicsTask);

	g_TS.AddTaskSetToPipe(s_physicsTask);
	g_TS.WaitforTask(&physicsTaskGroup);

	//end = Diagnostics::getTime();
	//std::cout << "sweep: " << (long double)Diagnostics::getDiff(start, end) << "ms" << std::endl << std::endl;

	return physicsTaskGroup.final_collisionList;
}

template std::vector<std::pair<int, int>>* PhysicsHandler::sweepAndPrune<Rect*>
(const std::vector<Rect*>& collider);
template std::vector<std::pair<int, int>>* PhysicsHandler::sweepAndPrune<Circle*>
(const std::vector<Circle*>& collider);
