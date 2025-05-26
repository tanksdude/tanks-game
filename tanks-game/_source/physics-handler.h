#pragma once
#include <utility>
#include <vector>
#include <unordered_map>

#include "circle.h"
#include "rect.h"
#include "game-thing.h"

#include <TaskScheduler.h>

class PhysicsHandler {
protected:
	struct ObjectIntervalInfo {
		double xStart;
		double xEnd;
		int listIndex;
		ObjectIntervalInfo() : ObjectIntervalInfo(0, 0, 0) {}
		ObjectIntervalInfo(double x1, double x2, int i) {
			xStart = x1;
			xEnd = x2;
			listIndex = i;
		}
	};

	struct SweepAndPruneTask : public enki::ITaskSet {
		std::vector<std::pair<int, int>>** m_collisionLists;
		const std::vector<PhysicsHandler::ObjectIntervalInfo>* m_objectIntervals;
		int num_threads;

		void Init(const std::vector<PhysicsHandler::ObjectIntervalInfo>* objectIntervals, int task_size);
		void ExecuteRange(enki::TaskSetPartition range_, uint32_t threadnum_) override;

		SweepAndPruneTask(int num_threads);
		~SweepAndPruneTask() override;
	};

	static uint32_t MinTaskSize;
	static SweepAndPruneTask* s_physicsTask;

public:
	static inline void Initialize() { Initialize(256); }
	static void Initialize(uint32_t MinTaskSize);
	static void Uninitialize();

	static std::vector<std::vector<std::pair<int, int>>*> sweepAndPrune(const std::vector<GameThing*>& list);

private:
	PhysicsHandler() = delete;
	PhysicsHandler(const PhysicsHandler&) = delete;
};
