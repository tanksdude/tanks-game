#pragma once
#include <utility>
#include <vector>
#include <unordered_map>

#include "circle.h"
#include "rect.h"

#include <TaskScheduler.h>

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

	static const uint32_t MinTaskSize;

	struct SweepAndPruneTask_TwoLists : public enki::ITaskSet {
		std::vector<std::pair<int, int>>** m_collisionLists;
		const std::vector<PhysicsHandler::ObjectIntervalInfo>* m_objectIntervals;
		int num_threads;

		//void Init(const std::vector<PhysicsHandler::ObjectIntervalInfo>* objectIntervals);
		void ExecuteRange(enki::TaskSetPartition range_, uint32_t threadnum_) override;

		SweepAndPruneTask_TwoLists(const std::vector<PhysicsHandler::ObjectIntervalInfo>* objectIntervals, int num_threads, int task_size);
		~SweepAndPruneTask_TwoLists() override;
	};

	struct SweepAndPruneTaskGroup_TwoLists : public enki::ITaskSet {
		SweepAndPruneTask_TwoLists*       m_testTasks;
		enki::Dependency                  m_Dependency;
		std::vector<std::pair<int, int>>* final_collisionList;

		void ExecuteRange(enki::TaskSetPartition range, uint32_t threadnum) override;

		SweepAndPruneTaskGroup_TwoLists(SweepAndPruneTask_TwoLists* testTask_);
		~SweepAndPruneTaskGroup_TwoLists() override;
	};

	struct SweepAndPruneTask : public enki::ITaskSet {
		std::vector<std::pair<int, int>>** m_collisionLists;
		const std::vector<PhysicsHandler::ObjectIntervalInfo>* m_objectIntervals;
		int num_threads;

		//void Init(const std::vector<PhysicsHandler::ObjectIntervalInfo>* objectIntervals);
		void ExecuteRange(enki::TaskSetPartition range_, uint32_t threadnum_) override;

		SweepAndPruneTask(const std::vector<PhysicsHandler::ObjectIntervalInfo>* objectIntervals, int num_threads, int task_size);
		~SweepAndPruneTask() override;
	};

	struct SweepAndPruneTaskGroup : public enki::ITaskSet {
		SweepAndPruneTask*                m_testTasks;
		enki::Dependency                  m_Dependency;
		std::vector<std::pair<int, int>>* final_collisionList;

		void ExecuteRange(enki::TaskSetPartition range, uint32_t threadnum) override;

		SweepAndPruneTaskGroup(SweepAndPruneTask* testTask_);
		~SweepAndPruneTaskGroup() override;
	};

public:
	template<typename T, typename U>
	static std::vector<std::pair<int, int>>* sweepAndPrune(const std::vector<T>& collider, const std::vector<U>& collidee);
	template<typename T>
	static std::vector<std::pair<int, int>>* sweepAndPrune(const std::vector<T>& collider);

private:
	PhysicsHandler() = delete;
	PhysicsHandler(const PhysicsHandler&) = delete;
};
