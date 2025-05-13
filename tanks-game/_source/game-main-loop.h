#pragma once
#include <string>

#include "game-scene.h"
#include "drawable-thing.h" //for DrawingLayers

#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>

#include "tank.h"
#include "bullet.h"
#include "wall.h"
#include "circle-hazard.h"
#include "rect-hazard.h"

struct TankInputChar {
protected:
	std::string key;
public:
	std::string getKey() const { return key; }
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
	void tankPowerTickAndCalculate();
	void bulletPowerTick();
	void tankToWall();
	void tankToHazard();
	void tankToTank();
	void tankToEdge();
	void bulletToEdge();
	void bulletToWall();
	void bulletToHazard();
	void bulletToBullet();
	void bulletToTank();

	void everythingToEverything();
	//void everythingToEverything_tank_tank(int i, int j, std::unordered_map<Game_ID, TankUpdateStruct>& tankUpdates);
	//void everythingToEverything_tank_wall(int i, int j, std::unordered_map<Game_ID, TankUpdateStruct>& tankUpdates, std::vector<Game_ID>& wallDeletionList, std::unordered_map<Game_ID, WallUpdateStruct>& wallUpdates);
	//void everythingToEverything_tank_circlehazard(int i, int j, std::unordered_map<Game_ID, TankUpdateStruct>& tankUpdates, std::vector<Game_ID>& circleHazardDeletionList, std::unordered_map<Game_ID, CircleHazardUpdateStruct>& circleHazardUpdates);
	//void everythingToEverything_tank_recthazard(int i, int j, std::unordered_map<Game_ID, TankUpdateStruct>& tankUpdates, std::vector<Game_ID>& rectHazardDeletionList, std::unordered_map<Game_ID, RectHazardUpdateStruct>& rectHazardUpdates);
	//void everythingToEverything_bullet_tank(int i, int j, std::vector<Game_ID>& bulletDeletionList, std::unordered_map<Game_ID, BulletUpdateStruct>& bulletUpdates, std::unordered_map<Game_ID, TankUpdateStruct>& tankUpdates);
	//void everythingToEverything_bullet_bullet(int i, int j, std::vector<Game_ID>& bulletDeletionList, std::unordered_map<Game_ID, BulletUpdateStruct>& bulletUpdates);
	//void everythingToEverything_bullet_wall(int i, int j, std::vector<Game_ID>& bulletDeletionList, std::unordered_map<Game_ID, BulletUpdateStruct>& bulletUpdates, std::vector<Game_ID>& wallDeletionList, std::unordered_map<Game_ID, WallUpdateStruct>& wallUpdates);
	//void everythingToEverything_bullet_circlehazard(int i, int j, std::vector<Game_ID>& bulletDeletionList, std::unordered_map<Game_ID, BulletUpdateStruct>& bulletUpdates, std::vector<Game_ID>& circleHazardDeletionList, std::unordered_map<Game_ID, CircleHazardUpdateStruct>& circleHazardUpdates);
	//void everythingToEverything_bullet_recthazard(int i, int j, std::vector<Game_ID>& bulletDeletionList, std::unordered_map<Game_ID, BulletUpdateStruct>& bulletUpdates, std::vector<Game_ID>& rectHazardDeletionList, std::unordered_map<Game_ID, RectHazardUpdateStruct>& rectHazardUpdates);

	void drawMain() const; //doesn't draw all layers (since not everything uses everything)
	void drawAllLayers() const;
	void drawLayer(DrawingLayers) const;

private:
	GameMainLoop(const GameMainLoop&) = delete;
};
