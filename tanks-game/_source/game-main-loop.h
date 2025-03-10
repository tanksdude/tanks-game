#pragma once
#include <string>

#include "game-scene.h"
#include "drawable-thing.h" //for DrawingLayers

#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl3.h"

struct TankInputChar {
protected:
	std::string key_name;
	ImGuiKey key_num;
public:
	std::string getKey() const { return key_name; }
	bool getKeyState() const;
	TankInputChar(std::string);
	TankInputChar();
};

enum class ThreadJobType {
	nothing,

	broad_bulletToWall,
	broad_bulletToCircleHazard,
	broad_bulletToRectHazard,
	broad_bulletToBullet,
	broad_bulletToTank,

	update_bullets,
	update_walls,
	update_circleHazards,
	update_rectHazards,
};

class GameMainLoop : public GameScene {
	friend class DeveloperManager;
	friend class GameManager; //needed?

	struct ThreadJob {
		ThreadJobType jobType;
		void* updateList;
		void* updateValues;
		void** newArrayPointer;
		int arrayStart, arrayEnd;
		ThreadJob(ThreadJobType j, void* list, void* values, int start, int end);
		ThreadJob(ThreadJobType j, void* list, void* values, void** newArr, int start, int end);
	private:
		ThreadJob();
		ThreadJob(const ThreadJob&);
	};

protected:
	int helperThreadCount;
	std::thread* thread_arr;
public:
	static std::atomic_bool keepRunning;
	static std::queue<GameMainLoop::ThreadJob*> workQueue; //TODO: real asynchronous consumer-producer queue
	static std::condition_variable queueCV;
	static std::mutex queueMutex;
	static std::atomic_bool* thread_isWorking;

	static void thread_func(int thread_id, int numThreads);

	static inline void thread_broadBulletToWall(void* bulletCollisionList, void* wallCollisionList, void** collisionPairList);
	static inline void thread_broadBulletToCircleHazard(void* bulletCollisionList, void* circleHazardCollisionList, void** collisionPairList);
	static inline void thread_broadBulletToRectHazard(void* bulletCollisionList, void* rectHazardCollisionList, void** collisionPairList);
	static inline void thread_broadBulletToBullet(void* bulletCollisionList, void** collisionPairList);
	static inline void thread_broadBulletToTank(void* bulletCollisionList, void* tankCollisionList, void** collisionPairList);

	static inline void thread_updateBulletsFunc(void* updateBulletList, void* updateBulletValues, int start, int end); //[start, end)
	static inline void thread_updateWallsFunc(void* updateWallList, void* updateWallValues, int start, int end); //probably pointless
	static inline void thread_updateCircleHazardsFunc(void* updateCircleHazardList, void* updateCircleHazardValues, int start, int end); //maybe pointless
	static inline void thread_updateRectHazardsFunc(void* updateRectHazardsList, void* updateRectHazardValues, int start, int end);

protected:
	int physicsRate; //(in Hz)
	int waitCount;
	int maxWaitCount;

public: //only for ResetThings
	//forward, turnL, turnR, shooting, specialKey
	TankInputChar tank1Inputs[5];
	TankInputChar tank2Inputs[5];

public:
	GameMainLoop();
	~GameMainLoop();
	void Tick() { Tick(physicsRate); }
	void Tick(int UPS) override;
	void Draw() const override { drawMain(); }

	//tick stuff:
	void levelTick();
	void moveTanks();
	void tankToPowerup();
	void tickHazards();
	void moveBullets();
	void tankShoot();
	void tankPowerCalculate();
	void bulletPowerCalculate();
	void tankToWall();
	void tankToHazard();
	void tankToTank();
	void tankToEdge();
	void bulletToEdge();
	void bulletToWall();
	void bulletToHazard();
	void bulletToBullet();
	void bulletToTank();

	void drawMain() const; //doesn't draw all layers (since not everything uses everything)
	void drawAllLayers() const;
	void drawLayer(DrawingLayers) const;

private:
	GameMainLoop(const GameMainLoop&) = delete;
};
