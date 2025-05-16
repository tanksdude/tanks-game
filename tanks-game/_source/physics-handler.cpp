#include "physics-handler.h"

#include <algorithm>
#include "aaa_first.h"

uint32_t PhysicsHandler::MinTaskSize;
PhysicsHandler::SweepAndPruneTask* PhysicsHandler::s_physicsTask;

void PhysicsHandler::Initialize(uint32_t MinTaskSize_) {
	MinTaskSize = MinTaskSize_;
	s_physicsTask = new SweepAndPruneTask(g_TS.GetNumTaskThreads());
}

void PhysicsHandler::Uninitialize() {
	delete s_physicsTask;
}

PhysicsHandler::SweepAndPruneTask::SweepAndPruneTask(int num_threads_) {
	(void) m_objectIntervals; (void) m_SetSize;
	num_threads = num_threads_;
	m_MinRange = PhysicsHandler::MinTaskSize;
	m_collisionLists = new std::vector<std::pair<int, int>>*[num_threads_];
	for (int i = 0; i < num_threads_; i++) {
		m_collisionLists[i] = new std::vector<std::pair<int, int>>;
		#if _DEBUG
		//performance seems about the same with or without reserving //TODO: check again
		#else
		m_collisionLists[i]->reserve(1024 * 1024); //random guess for what should be enough
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
			if (iteratingObjects[j].listIndex < range_.end) [[likely]] {
				//NOTE: this check only works because listIndex refers to the actual index in the list
				//(does not work when the list is sorted before calling this function)

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

std::vector<std::pair<int, int>>* PhysicsHandler::sweepAndPrune(const std::vector<GameThing*>& collisionObjects) {
	//find object intervals
	std::vector<ObjectIntervalInfo> objectIntervals; objectIntervals.reserve(collisionObjects.size());
	for (int i = 0; i < collisionObjects.size(); i++) {
		GameThing* o = collisionObjects[i];
		objectIntervals.push_back(ObjectIntervalInfo(o->get_xStart(), o->get_xEnd(), i));
	}

	//sweep through
	s_physicsTask->Init(&objectIntervals, collisionObjects.size());
	SweepAndPruneTaskGroup physicsTaskGroup(s_physicsTask);

	g_TS.AddTaskSetToPipe(s_physicsTask);
	g_TS.WaitforTask(&physicsTaskGroup);

	return physicsTaskGroup.final_collisionList;
}
