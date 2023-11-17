#include "game-main-loop.h"

//STL stuff:
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <iostream>

//some stuff:
#include "renderer.h"
#include "reset-things.h"
#include "keypress-manager.h"
//other:
#include "background-rect.h"
#include "diagnostics.h"

//managers:
#include "game-manager.h"
#include "tank-manager.h"
#include "bullet-manager.h"
#include "powerup-manager.h"
#include "wall-manager.h"
#include "level-manager.h"
#include "hazard-manager.h"

//classes with important handling functions:
#include "collision-handler.h"
#include "end-game-handler.h"
#include "physics-handler.h"

//#include <GL/glew.h>
//#include <GL/freeglut.h>

void doThing() {
	return;
}

TankInputChar::TankInputChar(std::string key_input) {
	key = key_input;
	if (KeypressManager::keyIsSpecialFromString(key_input)) {
		isSpecial = true;
		key_num = KeypressManager::specialKeyFromString(key_input);
	} else {
		isSpecial = false;
		key_num = KeypressManager::normalKeyFromString(key_input);
	}
}
TankInputChar::TankInputChar() {
	key = "`";
	isSpecial = false;
	key_num = '`';
}

bool TankInputChar::getKeyState() const {
	if (isSpecial) {
		return KeypressManager::getSpecialKey(key_num);
	}
	return KeypressManager::getNormalKey(key_num);
}

GameMainLoop::ThreadJob::ThreadJob(ThreadJobType j, void* list, void* values, void** newArr, int start, int end) {
	jobType = j;
	updateList = list;
	updateValues = values;
	newArrayPointer = newArr;
	arrayStart = start;
	arrayEnd = end;
}

GameMainLoop::ThreadJob::ThreadJob(ThreadJobType j, void* list, void* values, int start, int end)
: GameMainLoop::ThreadJob::ThreadJob(j, list, values, nullptr, start, end) {}

std::atomic_bool GameMainLoop::keepRunning;
std::queue<GameMainLoop::ThreadJob*> GameMainLoop::workQueue; //TODO: real asynchronous consumer-producer queue
std::condition_variable GameMainLoop::queueCV;
std::mutex GameMainLoop::queueMutex;
std::atomic_bool* GameMainLoop::thread_isWorking;

GameMainLoop::GameMainLoop() : GameScene() {
	const BasicINIParser::BasicINIData& ini_data = GameManager::get_INI();

	helperThreadCount = 0;
	if (ini_data.exists("UNIVERSAL", "ThreadCount")) {
		helperThreadCount = -1 + std::stoi(ini_data.get("UNIVERSAL", "ThreadCount")); //the ini lists total thread count, this only generates the extra threads
		helperThreadCount = std::max(helperThreadCount, 0);
	}

	thread_isWorking = new std::atomic_bool[helperThreadCount];
	thread_arr = new std::thread[helperThreadCount];
	for (int i = 0; i < helperThreadCount; i++) {
		thread_isWorking[i].store(false);
		thread_arr[i] = std::thread(thread_func, i, helperThreadCount+1);
	}

	physicsRate = 100;
	waitCount = 0;
	maxWaitCount = 1000/physicsRate * 10;
}

GameMainLoop::~GameMainLoop() {
	keepRunning.store(false);
	queueCV.notify_all();

	for (int i = 0; i < helperThreadCount; i++) {
		thread_arr[i].join();
	}
	delete[] thread_arr;
	delete[] thread_isWorking;

	while (!workQueue.empty()) {
		delete workQueue.front();
		workQueue.pop();
	}
}

void GameMainLoop::thread_func(int thread_id, int numThreads) {
	while (keepRunning.load()) {
		std::unique_lock<std::mutex> lock(queueMutex);
		while (keepRunning.load()) {
			queueCV.wait(lock);
		}
		if (!keepRunning) {
			//notify_all was called, thread needs to die
			//TODO: is the mutex supposed to be unlocked?
			break;
		}
		if (workQueue.size() == 0) {
			//thread accidentally woken up
			queueMutex.unlock();
			//std::this_thread::yield();
			continue;
		}

		GameMainLoop::ThreadJob* job = workQueue.front();
		workQueue.pop();
		thread_isWorking[thread_id].store(true);
		queueMutex.unlock();

		bool didWork = true;
		switch (job->jobType) {
			default:
				std::cerr << "unsupported job type " << int(job->jobType) << std::endl;
				[[fallthrough]];
			case ThreadJobType::nothing:
				didWork = false;
				break;

			case ThreadJobType::update_bullets:
				thread_updateBulletsFunc(job->updateList, job->updateValues, job->arrayStart, job->arrayEnd);
				break;
			case ThreadJobType::update_walls:
				thread_updateWallsFunc(job->updateList, job->updateValues, job->arrayStart, job->arrayEnd);
				break;
			case ThreadJobType::update_circleHazards:
				thread_updateCircleHazardsFunc(job->updateList, job->updateValues, job->arrayStart, job->arrayEnd);
				break;
			case ThreadJobType::update_rectHazards:
				thread_updateRectHazardsFunc(job->updateList, job->updateValues, job->arrayStart, job->arrayEnd);
				break;

			case ThreadJobType::broad_bulletToWall:
				thread_broadBulletToWall(job->updateList, job->updateValues, job->newArrayPointer);
				break;
			case ThreadJobType::broad_bulletToCircleHazard:
				thread_broadBulletToCircleHazard(job->updateList, job->updateValues, job->newArrayPointer);
				break;
			case ThreadJobType::broad_bulletToRectHazard:
				thread_broadBulletToRectHazard(job->updateList, job->updateValues, job->newArrayPointer);
				break;
			case ThreadJobType::broad_bulletToBullet:
				thread_broadBulletToBullet(job->updateList, job->newArrayPointer);
				break;
			case ThreadJobType::broad_bulletToTank:
				thread_broadBulletToTank(job->updateList, job->updateValues, job->newArrayPointer);
				break;
		}

		delete job;
		if (didWork) {
			thread_isWorking[thread_id].store(false);
		} else {
			continue;
		}
	}
	//make sure to join() to properly end the thread
}

inline void GameMainLoop::thread_broadBulletToWall(void* bulletCollisionList, void* wallCollisionList, void** collisionPairList) {
	*collisionPairList = PhysicsHandler::sweepAndPrune<Circle*, Rect*>(*((std::vector<Circle*>*) bulletCollisionList), *((std::vector<Rect*>*) wallCollisionList));
}

inline void GameMainLoop::thread_broadBulletToCircleHazard(void* bulletCollisionList, void* circleHazardCollisionList, void** collisionPairList) {
	*collisionPairList = PhysicsHandler::sweepAndPrune<Circle*, Circle*>(*((std::vector<Circle*>*) bulletCollisionList), *((std::vector<Circle*>*) circleHazardCollisionList));
}

inline void GameMainLoop::thread_broadBulletToRectHazard(void* bulletCollisionList, void* rectHazardCollisionList, void** collisionPairList) {
	*collisionPairList = PhysicsHandler::sweepAndPrune<Circle*, Rect*>(*((std::vector<Circle*>*) bulletCollisionList), *((std::vector<Rect*>*) rectHazardCollisionList));
}

inline void GameMainLoop::thread_broadBulletToBullet(void* bulletCollisionList, void** collisionPairList) {
	*collisionPairList = PhysicsHandler::sweepAndPrune<Circle*>(*((std::vector<Circle*>*) bulletCollisionList));
}

inline void GameMainLoop::thread_broadBulletToTank(void* bulletCollisionList, void* tankCollisionList, void** collisionPairList) {
	*collisionPairList = PhysicsHandler::sweepAndPrune<Circle*, Circle*>(*((std::vector<Circle*>*) bulletCollisionList), *((std::vector<Circle*>*) tankCollisionList));
}

inline void GameMainLoop::thread_updateBulletsFunc(void* updateBulletList, void* updateBulletValues, int start, int end) {
	std::unordered_map<Game_ID, BulletUpdateStruct>* bulletUpdates = (std::unordered_map<Game_ID, BulletUpdateStruct>*) updateBulletValues;
	std::vector<Game_ID>* bulletUpdateList = (std::vector<Game_ID>*) updateBulletList;

	for (int i = start; i < end; i++) {
		Bullet* b = BulletManager::getBulletByID(bulletUpdateList->at(i));
		b->update(&bulletUpdates->at(bulletUpdateList->at(i)));
	}
}

inline void GameMainLoop::thread_updateWallsFunc(void* updateWallList, void* updateWallValues, int start, int end) {
	std::unordered_map<Game_ID, WallUpdateStruct>* wallUpdates = (std::unordered_map<Game_ID, WallUpdateStruct>*) updateWallValues;
	std::vector<Game_ID>* wallUpdateList = (std::vector<Game_ID>*) updateWallList;
	
	for (int i = start; i < end; i++) {
		Wall* w = WallManager::getWallByID(wallUpdateList->at(i));
		w->update(&wallUpdates->at(wallUpdateList->at(i)));
		//TODO: is there still a unordered_map invalid key error here?
	}
}

inline void GameMainLoop::thread_updateCircleHazardsFunc(void* updateCircleHazardList, void* updateCircleHazardValues, int start, int end) {
	/*
	std::unordered_map<Game_ID, CircleHazardUpdateStruct>* wallUpdates = (std::unordered_map<Game_ID, CircleHazardUpdateStruct>*) updateCircleHazardValues;
	std::vector<Game_ID>* circleHazardUpdateList = (std::vector<Game_ID>*) updateCircleHazardList;

	for (int i = start; i < end; i++) {
		CircleHazard* ch = HazardManager::getCircleHazardByID(circleHazardUpdateList->at(i));
		ch->update(&circleHazardUpdates->at(circleHazardUpdateList->at(i)));
	}
	*/
}

inline void GameMainLoop::thread_updateRectHazardsFunc(void* updateRectHazardsList, void* updateRectHazardValues, int start, int end) {
	/*
	std::unordered_map<Game_ID, RectHazardUpdateStruct>* wallUpdates = (std::unordered_map<Game_ID, RectHazardUpdateStruct>*) updateRectHazardValues;
	std::vector<Game_ID>* rectHazardUpdateList = (std::vector<Game_ID>*) updateRectHazardList;

	for (int i = start; i < end; i++) {
		RectHazard* rh = HazardManager::getRectHazardByID(rectHazardUpdateList->at(i));
		rh->update(&rectHazardUpdates->at(rectHazardUpdateList->at(i)));
	}
	*/
}

void GameMainLoop::Tick(int UPS) {
	if (EndGameHandler::shouldGameEnd()) {
		waitCount++;
		if (waitCount >= maxWaitCount) {
			waitCount = 0;
			ResetThings::reset();
		}
		return;
	}

	//big problem: I currently don't have the brainpower or willingness to parallelize the broad phase of collision, but I need to multithread something
	//solution: parallelize the various broad phases!
	//bonus: this is how all collision will be able to be done simultaneously

	auto start = Diagnostics::getTime();
	doThing();

	//level stuff:
	levelTick();

	//move tanks:
	moveTanks();

	//collide tanks with powerups:
	Diagnostics::startTiming("tank-powerups");
	tankToPowerup();
	Diagnostics::endTiming();

	//tick hazards:
	tickHazards();

	//move bullets:
	moveBullets();

	//powerCalculate on tanks and bullets, then tank shoot:
	Diagnostics::startTiming("power calculate and tank shoot");
	tankShoot();
	tankPowerCalculate();
	bulletPowerCalculate();
	Diagnostics::endTiming();

	//collide tanks with walls:
	//this comes before powerCalculate stuff in JS Tanks; TODO: should this be changed?
	Diagnostics::startTiming("tank-walls");
	tankToWall();
	Diagnostics::endTiming();

	//collide tanks with hazards:
	Diagnostics::startTiming("tank-hazards");
	tankToHazard();
	Diagnostics::endTiming();

	//collide tanks with tanks:
	Diagnostics::startTiming("tank-tank");
	tankToTank();
	Diagnostics::endTiming();

	//collide tanks against edges:
	tankToEdge();

	//collide bullets against edges:
	Diagnostics::startTiming("bullet-edge");
	bulletToEdge();
	Diagnostics::endTiming();

	//collide bullets with walls:
	Diagnostics::startTiming("bullet-wall");
	bulletToWall();
	Diagnostics::endTiming();

	//collide bullets with hazards:
	Diagnostics::startTiming("bullet-hazards");
	bulletToHazard();
	Diagnostics::endTiming();

	//collide bullets with bullets:
	Diagnostics::startTiming("bullet-bullet");
	bulletToBullet();
	Diagnostics::endTiming();
	//add another shader: main uses proj, modify doesn't

	//collide bullets with tanks:
	Diagnostics::startTiming("bullet-tank");
	bulletToTank();
	Diagnostics::endTiming();

	//edge constrain tanks again in case bullet decided to move tank
	//don't edge constrain if said tank is dead //TODO: implement
	/*
	tankToEdge();
	*/

	//finish up by incrementing the tick count
	GameManager::Tick();

	auto end = Diagnostics::getTime();
	Diagnostics::pushGraphTime("tick", Diagnostics::getDiff(start, end));

	//Diagnostics::printPreciseTimings();
	Diagnostics::clearTimes();

	//std::cout << BulletManager::getNumBullets() << std::endl;
	//std::cout << "tick: " << (long long)Diagnostics::getDiff(start, end) << "ms" << std::endl << std::endl;

	//largest timesinks:
	//1. bullet-bullet
	//2. bullet-hazard
	//2. bullet-wall (really depends on how many hazards/walls)
	//3. power calculate and tank shoot
}

void GameMainLoop::levelTick() {
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->tickLevelEffects();
	}
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->doLevelEffects();
		LevelManager::getLevel(i)->tick();
	}
}

void GameMainLoop::moveTanks() {
	/*
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		Tank* t = TankManager::getTank(i);
		bool shouldBeKilled = t->move();
		if (shouldBeKilled) {
			EndGameHandler::killTank(t);
		}
	}
	*/

	//TODO: make this a loop (not done right now 'cuz I don't want to do more arrays than I absolutely have to)
	Tank* t = TankManager::getTank(0);
	bool shouldBeKilled = t->move(tank1Inputs[0].getKeyState(), tank1Inputs[1].getKeyState(), tank1Inputs[2].getKeyState(), tank1Inputs[4].getKeyState());
	if (shouldBeKilled) {
		EndGameHandler::killTank(t);
	}
	t = TankManager::getTank(1);
	shouldBeKilled = t->move(tank2Inputs[0].getKeyState(), tank2Inputs[1].getKeyState(), tank2Inputs[2].getKeyState(), tank2Inputs[4].getKeyState());
	if (shouldBeKilled) {
		EndGameHandler::killTank(t);
	}
}

void GameMainLoop::tankToPowerup() {
	//broad phase
	std::vector<std::pair<int, int>>* collisionList = PhysicsHandler::sweepAndPrune<Circle*, Rect*>(TankManager::getTankCollisionList(), PowerupManager::getPowerupCollisionList());

	//narrow phase and resolve collision
	//std::vector<Game_ID> tankDeletionList; //custom tank-powerup collision doesn't exist
	std::vector<Game_ID> powerupDeletionList;
	for (int i = 0; i < collisionList->size(); i++) {
		std::pair<int, int> collisionPair = collisionList->at(i);
		Tank* t = TankManager::getTank(collisionPair.first);
		PowerSquare* p = PowerupManager::getPowerup(collisionPair.second);

		if (CollisionHandler::partiallyCollided(p, t)) {
			p->givePower(t);
			powerupDeletionList.push_back(p->getGameID());
		}
	}

	//clear deaths
	for (int i = powerupDeletionList.size() - 1; i >= 0; i--) {
		PowerupManager::deletePowerupByID(powerupDeletionList[i]);
	}

	delete collisionList;
}

void GameMainLoop::tickHazards() {
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		HazardManager::getCircleHazard(i)->tick();
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		HazardManager::getRectHazard(i)->tick();
	}
}

void GameMainLoop::moveBullets() {
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b = BulletManager::getBullet(i);
		bool shouldBeKilled = b->move();
		if (shouldBeKilled) {
			EndGameHandler::killBullet(b);
			BulletManager::deleteBullet(i);
			continue;
			//TODO: should all the actual deletions go at the end of everything?
		}
	}

	BulletManager::forceLimitBullets();
}

void GameMainLoop::tankPowerCalculate() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->powerCalculate();
	}
}

void GameMainLoop::bulletPowerCalculate() {
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b = BulletManager::getBullet(i);
		b->powerCalculate();
		if (b->isDead()) {
			BulletManager::deleteBullet(i);
			continue;
		}
	}
}

void GameMainLoop::tankShoot() {
	/*
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->shoot();
	}
	*/

	//TODO: loop
	TankManager::getTank(0)->shoot(tank1Inputs[3].getKeyState());
	TankManager::getTank(1)->shoot(tank2Inputs[3].getKeyState());
}

void GameMainLoop::tankToWall() {
	//broad phase
	std::vector<std::pair<int, int>>* collisionList = PhysicsHandler::sweepAndPrune<Circle*, Rect*>(TankManager::getTankCollisionList(), WallManager::getWallCollisionList());

	//narrow phase and resolve collision
	//std::vector<Game_ID> tankDeletionList;
	std::vector<Game_ID> wallDeletionList;
	for (int i = 0; i < collisionList->size(); i++) {
		std::pair<int, int> collisionPair = collisionList->at(i);
		Tank* t = TankManager::getTank(collisionPair.first);
		bool shouldBeKilled = false; //unlikely to be set

		Wall* w = WallManager::getWall(collisionPair.second);
		bool killWall = false;
		bool modifiedWallCollision = false;
		bool overridedWallCollision = false;

		if (CollisionHandler::partiallyCollided(t, w)) {
			for (int k = 0; k < t->tankPowers.size(); k++) {
				if (t->tankPowers[k]->modifiesCollisionWithWall) {
					if (t->tankPowers[k]->modifiedCollisionWithWallCanOnlyWorkIndividually && modifiedWallCollision) {
						continue;
					}
					modifiedWallCollision = true;
					if (t->tankPowers[k]->overridesCollisionWithWall) {
						overridedWallCollision = true;
					}

					InteractionBoolHolder check_temp = t->tankPowers[k]->modifiedCollisionWithWall(t, w);
					if (check_temp.shouldDie) {
						shouldBeKilled = true;
					}
					if (check_temp.otherShouldDie) {
						killWall = true;
					}

					if (!t->tankPowers[k]->modifiedEdgeCollisionCanWorkWithOthers) {
						break;
					}
				}
			}

			if (!overridedWallCollision) {
				if (CollisionHandler::partiallyCollided(t, w)) {
					InteractionBoolHolder result = EndGameHandler::determineWinner(t, w, shouldBeKilled);
					if (result.shouldDie) {
						shouldBeKilled = true;
					}
					if (result.otherShouldDie) {
						killWall = true;
					}
				}
			}
		}

		if (killWall) {
			wallDeletionList.push_back(w->getGameID());
		}

		if (shouldBeKilled) {
			//TODO: proper implementation?
		}
	}

	//clear deaths
	for (int i = wallDeletionList.size() - 1; i >= 0; i--) {
		WallManager::deleteWallByID(wallDeletionList[i]);
	}

	delete collisionList;
}

void GameMainLoop::tankToHazard() {
	//broad phase
	std::vector<std::pair<int, int>>* collisionList = PhysicsHandler::sweepAndPrune<Circle*, Circle*>(TankManager::getTankCollisionList(), HazardManager::getCircleHazardCollisionList());

	//narrow phase and resolve collision
	//std::vector<Game_ID> tankDeletionList;
	std::vector<Game_ID> circleHazardDeletionList;
	for (int i = 0; i < collisionList->size(); i++) {
		std::pair<int, int> collisionPair = collisionList->at(i);
		Tank* t = TankManager::getTank(collisionPair.first);
		bool shouldBeKilled = false;

		//circles:
		CircleHazard* ch = HazardManager::getCircleHazard(collisionPair.second);
		bool killCircleHazard = false;
		bool modifiedCircleHazardCollision = false;
		bool overridedCircleHazardCollision = false;

		if (CollisionHandler::partiallyCollided(t, ch)) {
			//tankpower decides whether to use the partial collision or the true collision
			for (int k = 0; k < t->tankPowers.size(); k++) {
				if (t->tankPowers[k]->getModifiesCollisionWithCircleHazard(ch)) {
					if (t->tankPowers[k]->modifiedCollisionWithCircleHazardCanOnlyWorkIndividually && modifiedCircleHazardCollision) {
						continue;
					}
					modifiedCircleHazardCollision = true;
					if (t->tankPowers[k]->overridesCollisionWithCircleHazard) {
						overridedCircleHazardCollision = true;
					}

					//TODO: this doesn't kill the tank but it should
					InteractionBoolHolder check_temp = t->tankPowers[k]->modifiedCollisionWithCircleHazard(t, ch);
					if (check_temp.shouldDie) {
						shouldBeKilled = true;
					}
					if (check_temp.otherShouldDie) {
						killCircleHazard = true;
					}

					if (!t->tankPowers[k]->modifiedCollisionWithCircleHazardCanWorkWithOthers) {
						break;
					}
				}
			}

			if (!overridedCircleHazardCollision) {
				if (CollisionHandler::partiallyCollided(t, ch)) {
					if (ch->actuallyCollided(t)) {
						InteractionBoolHolder result = EndGameHandler::determineWinner(t, ch);
						if (result.shouldDie) {
							shouldBeKilled = true;
						}
						if (result.otherShouldDie) {
							killCircleHazard = true;
						}
					}
				}
			}
		}

		if (killCircleHazard) {
			circleHazardDeletionList.push_back(ch->getGameID());
		}

		if (shouldBeKilled) {
			//TODO: proper implementation?
		}
	}

	delete collisionList;

	//broad phase
	collisionList = PhysicsHandler::sweepAndPrune<Circle*, Rect*>(TankManager::getTankCollisionList(), HazardManager::getRectHazardCollisionList());

	//narrow phase and resolve collision
	std::vector<Game_ID> rectHazardDeletionList;
	for (int i = 0; i < collisionList->size(); i++) {
		std::pair<int, int> collisionPair = collisionList->at(i);
		Tank* t = TankManager::getTank(collisionPair.first);
		bool shouldBeKilled = false;

		//rectangles:
		RectHazard* rh = HazardManager::getRectHazard(collisionPair.second);
		bool killRectHazard = false;
		bool modifiedRectHazardCollision = false;
		bool overridedRectHazardCollision = false;

		if (CollisionHandler::partiallyCollided(t, rh)) {
			for (int k = 0; k < t->tankPowers.size(); k++) {
				if (t->tankPowers[k]->getModifiesCollisionWithRectHazard(rh)) {
					if (t->tankPowers[k]->modifiedCollisionWithRectHazardCanOnlyWorkIndividually && modifiedRectHazardCollision) {
						continue;
					}
					modifiedRectHazardCollision = true;
					if (t->tankPowers[k]->overridesCollisionWithRectHazard) {
						overridedRectHazardCollision = true;
					}

					//TODO: this doesn't kill the tank but it should
					InteractionBoolHolder check_temp = t->tankPowers[k]->modifiedCollisionWithRectHazard(t, rh);
					if (check_temp.shouldDie) {
						shouldBeKilled = true;
					}
					if (check_temp.otherShouldDie) {
						killRectHazard = true;
					}

					if (!t->tankPowers[k]->modifiedCollisionWithRectHazardCanWorkWithOthers) {
						break;
					}
				}
			}

			if (!overridedRectHazardCollision) {
				if (CollisionHandler::partiallyCollided(t, rh)) {
					if (rh->actuallyCollided(t)) {
						InteractionBoolHolder result = EndGameHandler::determineWinner(t, rh);
						if (result.shouldDie) {
							shouldBeKilled = true;
						}
						if (result.otherShouldDie) {
							killRectHazard = true;
						}
					}
				}
			}
		}

		if (killRectHazard) {
			rectHazardDeletionList.push_back(rh->getGameID());
		}

		if (shouldBeKilled) {
			//TODO: proper implementation?
		}
	}

	//clear deaths
	for (int i = circleHazardDeletionList.size() - 1; i >= 0; i--) {
		HazardManager::deleteCircleHazardByID(circleHazardDeletionList[i]);
	}
	for (int i = rectHazardDeletionList.size() - 1; i >= 0; i--) {
		HazardManager::deleteRectHazardByID(rectHazardDeletionList[i]);
	}

	delete collisionList;
}

void GameMainLoop::tankToTank() {
	//doesn't really need sweep and prune, but will need to be updated to avoid one tank getting preference over the other
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		Tank* t_outer = TankManager::getTank(i);
		bool t_outerShouldDie = false;

		for (int j = 0; j < TankManager::getNumTanks(); j++) {
			if (i == j) {
				continue;
			}

			Tank* t_inner = TankManager::getTank(j);
			bool t_innerShouldDie = false;
			bool modifiedTankCollision = false;
			bool overridedTankCollision = false;

			if (CollisionHandler::partiallyCollided(t_outer, t_inner)) {
				for (int k = 0; k < t_outer->tankPowers.size(); k++) {
					if (t_outer->tankPowers[k]->modifiesCollisionWithWall) {
						if (t_outer->tankPowers[k]->modifiedCollisionWithTankCanOnlyWorkIndividually && modifiedTankCollision) {
							continue;
						}
						modifiedTankCollision = true;
						if (t_outer->tankPowers[k]->overridesCollisionWithTank) {
							overridedTankCollision = true;
						}

						InteractionBoolHolder check_temp = t_outer->tankPowers[k]->modifiedCollisionWithTank(t_outer, t_inner);
						if (check_temp.shouldDie) {
							t_outerShouldDie = true;
						}
						if (check_temp.otherShouldDie) {
							if (t_outer->getTeamID() != t_inner->getTeamID()) {
								t_innerShouldDie = true;
							}
						}

						if (!t_outer->tankPowers[k]->modifiedCollisionWithTankCanWorkWithOthers) {
							break;
						}
					}
				}

				if (!overridedTankCollision) {
					InteractionBoolHolder result = EndGameHandler::determineWinner(t_outer, t_inner);
					t_outerShouldDie = result.shouldDie;
					t_innerShouldDie = result.otherShouldDie;
				}
			}

			if (t_innerShouldDie) {
				//TODO: proper implementation?
			}
		}

		if (t_outerShouldDie) {
			//TODO: proper implementation?
		}
	}
}

void GameMainLoop::tankToEdge() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		Tank* t = TankManager::getTank(i);
		bool shouldBeKilled = false;
		bool modifiedEdgeCollision = false;
		bool overridedEdgeCollision = false;

		if (CollisionHandler::partiallyOutOfBounds(t)) {
			for (int k = 0; k < t->tankPowers.size(); k++) {
				if (t->tankPowers[k]->modifiesEdgeCollision) {
					if (t->tankPowers[k]->modifiedEdgeCollisionCanOnlyWorkIndividually && modifiedEdgeCollision) {
						continue;
					}
					modifiedEdgeCollision = true;
					if (t->tankPowers[k]->overridesEdgeCollision) {
						overridedEdgeCollision = true;
					}

					InteractionBoolHolder check_temp = t->tankPowers[k]->modifiedEdgeCollision(t);
					if (check_temp.shouldDie) {
						shouldBeKilled = true;
					}

					if (!t->tankPowers[k]->modifiedEdgeCollisionCanWorkWithOthers) {
						break;
					}
				}
			}

			if (!overridedEdgeCollision) {
				CollisionHandler::edgeConstrain(t);
			}
		}

		if (shouldBeKilled) {
			//TODO: proper implementation?
		}
	}
}

void GameMainLoop::bulletToEdge() {
	for (int i = BulletManager::getNumBullets() - 1; i >= 0; i--) {
		Bullet* b = BulletManager::getBullet(i);
		bool shouldBeKilled = false;
		bool modifiedEdgeCollision = false;
		bool overridedEdgeCollision = false;

		if (CollisionHandler::partiallyOutOfBounds(b)) {
			for (int k = 0; k < b->bulletPowers.size(); k++) {
				if (b->bulletPowers[k]->modifiesEdgeCollision) {
					if (b->bulletPowers[k]->modifiedEdgeCollisionCanOnlyWorkIndividually && modifiedEdgeCollision) {
						continue;
					}
					modifiedEdgeCollision = true;
					if (b->bulletPowers[k]->overridesEdgeCollision) {
						overridedEdgeCollision = true;
					}

					InteractionBoolHolder check_temp = b->bulletPowers[k]->modifiedEdgeCollision(b);
					if (check_temp.shouldDie) {
						shouldBeKilled = true;
					}

					if (!b->bulletPowers[k]->modifiedEdgeCollisionCanWorkWithOthers) {
						break;
					}
				}
			}

			if (!overridedEdgeCollision) {
				if (CollisionHandler::fullyOutOfBounds(b)) {
					shouldBeKilled = true;
				}
			}
		}

		if (shouldBeKilled) {
			BulletManager::deleteBullet(i);
			continue;
		}
	}
}

void GameMainLoop::bulletToWall() {
	//broad phase
	std::vector<std::pair<int, int>>* collisionList = PhysicsHandler::sweepAndPrune<Circle*, Rect*>(BulletManager::getBulletCollisionList(), WallManager::getWallCollisionList());

	//narrow phase and resolve collision
	std::vector<Game_ID> bulletDeletionList;
	std::vector<Game_ID> wallDeletionList;

	std::unordered_map<Game_ID, BulletUpdateStruct> bulletUpdates; //TODO: what if a bullet wants to make more of itself when it hits a wall?
	std::vector<Game_ID> bulletUpdateList;
	std::unordered_map<Game_ID, WallUpdateStruct> wallUpdates; //bullets probably shouldn't be able to hit a wall and make more walls
	std::vector<Game_ID> wallUpdateList;

	for (int i = 0; i < collisionList->size(); i++) {
		std::pair<int, int> collisionPair = collisionList->at(i);
		Bullet* b = BulletManager::getBullet(collisionPair.first);
		bool shouldBeKilled = false;

		Wall* w = WallManager::getWall(collisionPair.second);
		bool killWall = false;
		bool modifiedWallCollision = false;
		bool overridedWallCollision = false;

		if (CollisionHandler::partiallyCollided(b, w)) {
			for (int k = 0; k < b->bulletPowers.size(); k++) {
				if (b->bulletPowers[k]->modifiesCollisionWithWall) {
					if (b->bulletPowers[k]->modifiedCollisionWithWallCanOnlyWorkIndividually && modifiedWallCollision) {
						continue;
					}
					modifiedWallCollision = true;
					if (b->bulletPowers[k]->overridesCollisionWithWall) {
						overridedWallCollision = true;
					}

					InteractionUpdateHolder<BulletUpdateStruct, WallUpdateStruct> check_temp = b->bulletPowers[k]->modifiedCollisionWithWall(b, w);
					if (check_temp.deaths.shouldDie) {
						shouldBeKilled = true;
					}
					if (check_temp.deaths.otherShouldDie) {
						killWall = true;
					}

					//TODO: maybe package all the updates together, then do this upsert
					if (check_temp.firstUpdate != nullptr) {
						if (bulletUpdates.find(b->getGameID()) == bulletUpdates.end()) {
							bulletUpdates.insert({ b->getGameID(), BulletUpdateStruct(*check_temp.firstUpdate) });
							bulletUpdateList.push_back(b->getGameID());
						} else {
							bulletUpdates[b->getGameID()].add(BulletUpdateStruct(*check_temp.firstUpdate));
						}
					}
					if (check_temp.secondUpdate != nullptr) {
						if (wallUpdates.find(w->getGameID()) == wallUpdates.end()) {
							wallUpdates.insert({ w->getGameID(), WallUpdateStruct(*check_temp.secondUpdate) });
							wallUpdateList.push_back(w->getGameID());
						} else {
							wallUpdates[w->getGameID()].add(WallUpdateStruct(*check_temp.secondUpdate));
						}
					}

					if (!b->bulletPowers[k]->modifiedCollisionWithWallCanWorkWithOthers) {
						break;
					}
				}
			}

			if (!overridedWallCollision) {
				if (CollisionHandler::partiallyCollided(b, w)) {
					shouldBeKilled = true;
				}
			}
		}

		if (killWall) {
			//wallDeletionList.push_back(collisionPair.second);
			wallDeletionList.push_back(w->getGameID());
		}

		if (shouldBeKilled) {
			//bulletDeletionList.push_back(collisionPair.first);
			bulletDeletionList.push_back(b->getGameID());
		}
	}

	/*
	//this is not demanding enough to multithread
	for (int i = 0; i < GameMainLoop::helperThreadCount; i++) {
		int start =   i * bulletUpdateList.size() / (helperThreadCount+1);
		int end = (i+1) * bulletUpdateList.size() / (helperThreadCount+1);
		queueMutex.lock();
		GameMainLoop::ThreadJob* job = new GameMainLoop::ThreadJob(ThreadJobType::update_bullets, &bulletUpdateList, &bulletUpdates, start, end);
		workQueue.push(job);
		queueCV.notify_one();
		queueMutex.unlock();
	}
	thread_updateBulletsFunc(&bulletUpdateList, &bulletUpdates, (helperThreadCount-1) * bulletUpdateList.size() / (helperThreadCount+1), bulletUpdates.size());
	*/

	thread_updateBulletsFunc(&bulletUpdateList, &bulletUpdates, 0, bulletUpdates.size());
	thread_updateWallsFunc(&wallUpdateList, &wallUpdates, 0, wallUpdateList.size()); //should go below for "clean code" but should stay here for thread scheduling reasons

	/*
	for (int i = 0; i < GameMainLoop::helperThreadCount; i++) {
		while (thread_isWorking[i].load()) {} //if you don't join, this is how you wait for the thread to finish
	}
	*/

	//clear deaths
	//std::sort(bulletDeletionList.begin(), bulletDeletionList.end());
	for (int i = bulletDeletionList.size() - 1; i >= 0; i--) {
		//BulletManager::deleteBullet(bulletDeletionList[i]);
		BulletManager::deleteBulletByID(bulletDeletionList[i]);
	}
	//std::sort(wallDeletionList.begin(), wallDeletionList.end());
	for (int i = wallDeletionList.size() - 1; i >= 0; i--) {
		WallManager::deleteWallByID(wallDeletionList[i]);
	}

	delete collisionList;
}

void GameMainLoop::bulletToHazard() {
	//broad phase
	std::vector<std::pair<int, int>>* collisionList = PhysicsHandler::sweepAndPrune<Circle*, Circle*>(BulletManager::getBulletCollisionList(), HazardManager::getCircleHazardCollisionList());

	//narrow phase and resolve collision
	std::vector<Game_ID> bulletDeletionList;
	std::vector<Game_ID> circleHazardDeletionList;
	for (int i = 0; i < collisionList->size(); i++) {
		std::pair<int, int> collisionPair = collisionList->at(i);
		Bullet* b = BulletManager::getBullet(collisionPair.first);
		bool shouldBeKilled = false;

		//circles:
		CircleHazard* ch = HazardManager::getCircleHazard(collisionPair.second);
		bool killCircleHazard = false;
		bool modifiedCircleHazardCollision = false;
		bool overridedCircleHazardCollision = false;

		if (!b->canCollideWith(ch)) {
			continue;
		}
		if (CollisionHandler::partiallyCollided(b, ch)) {
			for (int k = 0; k < b->bulletPowers.size(); k++) {
				if (b->bulletPowers[k]->getModifiesCollisionWithCircleHazard(ch)) {
					if (b->bulletPowers[k]->modifiedCollisionWithCircleHazardCanOnlyWorkIndividually && modifiedCircleHazardCollision) {
						continue;
					}
					modifiedCircleHazardCollision = true;
					if (b->bulletPowers[k]->overridesCollisionWithCircleHazard) {
						overridedCircleHazardCollision = true;
					}

					InteractionBoolHolder check_temp = b->bulletPowers[k]->modifiedCollisionWithCircleHazard(b, ch);
					if (check_temp.shouldDie) {
						shouldBeKilled = true;
					}
					if (check_temp.otherShouldDie) {
						killCircleHazard = true;
					}

					if (!b->bulletPowers[k]->modifiedCollisionWithCircleHazardCanWorkWithOthers) {
						break;
					}
				}
			}

			if (!overridedCircleHazardCollision) {
				if (CollisionHandler::partiallyCollided(b, ch)) {
					if (ch->actuallyCollided(b)) {
						InteractionBoolHolder result = EndGameHandler::determineWinner(b, ch);
						if (result.shouldDie) {
							shouldBeKilled = true;
						}
						if (result.otherShouldDie) {
							killCircleHazard = true;
						}
					}
				}
			}
		}

		if (killCircleHazard) {
			circleHazardDeletionList.push_back(ch->getGameID());
		}

		if (shouldBeKilled) {
			bulletDeletionList.push_back(b->getGameID());
			//it's possible to add the same bullet twice, but deleting by ID doesn't care about that
		}
	}

	delete collisionList;

	//broad phase
	collisionList = PhysicsHandler::sweepAndPrune<Circle*, Rect*>(BulletManager::getBulletCollisionList(), HazardManager::getRectHazardCollisionList());

	//narrow phase and resolve collision
	std::vector<Game_ID> rectHazardDeletionList;
	for (int i = 0; i < collisionList->size(); i++) {
		std::pair<int, int> collisionPair = collisionList->at(i);
		Bullet* b = BulletManager::getBullet(collisionPair.first);
		bool shouldBeKilled = false;

		//rectangles:
		RectHazard* rh = HazardManager::getRectHazard(collisionPair.second);
		bool killRectHazard = false;
		bool modifiedRectHazardCollision = false;
		bool overridedRectHazardCollision = false;

		if (!b->canCollideWith(rh)) {
			continue;
		}
		if (CollisionHandler::partiallyCollided(b, rh)) {
			for (int k = 0; k < b->bulletPowers.size(); k++) {
				if (b->bulletPowers[k]->getModifiesCollisionWithRectHazard(rh)) {
					if (b->bulletPowers[k]->modifiedCollisionWithRectHazardCanOnlyWorkIndividually && modifiedRectHazardCollision) {
						continue;
					}
					modifiedRectHazardCollision = true;
					if (b->bulletPowers[k]->overridesCollisionWithRectHazard) {
						overridedRectHazardCollision = true;
					}

					InteractionBoolHolder check_temp = b->bulletPowers[k]->modifiedCollisionWithRectHazard(b, rh);
					if (check_temp.shouldDie) {
						shouldBeKilled = true;
					}
					if (check_temp.otherShouldDie) {
						killRectHazard = true;
					}

					if (!b->bulletPowers[k]->modifiedCollisionWithRectHazardCanWorkWithOthers) {
						break;
					}
				}
			}

			if (!overridedRectHazardCollision) {
				if (CollisionHandler::partiallyCollided(b, rh)) {
					if (rh->actuallyCollided(b)) {
						InteractionBoolHolder result = EndGameHandler::determineWinner(b, rh);
						if (result.shouldDie) {
							shouldBeKilled = true;
						}
						if (result.otherShouldDie) {
							killRectHazard = true;
						}
					}
				}
			}
		}

		if (killRectHazard) {
			rectHazardDeletionList.push_back(rh->getGameID());
		}

		if (shouldBeKilled) {
			bulletDeletionList.push_back(b->getGameID());
			//it's possible to add the same bullet twice, but deleting by ID doesn't care about that
		}
	}

	//clear deaths
	for (int i = bulletDeletionList.size() - 1; i >= 0; i--) {
		BulletManager::deleteBulletByID(bulletDeletionList[i]);
	}
	for (int i = circleHazardDeletionList.size() - 1; i >= 0; i--) {
		HazardManager::deleteCircleHazardByID(circleHazardDeletionList[i]);
	}
	for (int i = rectHazardDeletionList.size() - 1; i >= 0; i--) {
		HazardManager::deleteRectHazardByID(rectHazardDeletionList[i]);
	}

	delete collisionList;
}

void GameMainLoop::bulletToBullet() {
	//TODO: modernize (add default vs custom collision stuff)
	//broad phase
	std::vector<std::pair<int, int>>* collisionList = PhysicsHandler::sweepAndPrune<Circle*>(BulletManager::getBulletCollisionList());
	//std::cout << "old: " << (BulletManager::getNumBullets() * BulletManager::getNumBullets()) << ", new: " << collisionList.size() << std::endl;
	//this is the largest timesink
	//TODO: this returns a large list only to get destroyed when the function ends; any way to reduce this memory footprint?

	//narrow phase and resolve collision
	std::vector<Game_ID> bulletDeletionList;

	std::unordered_map<Game_ID, BulletUpdateStruct> bulletUpdates; //yeah, this is unused...
	std::vector<Game_ID> bulletUpdateList;

	for (int i = 0; i < collisionList->size(); i++) {
		std::pair<int, int> collisionPair = collisionList->at(i);
		Bullet* b_outer = BulletManager::getBullet(collisionPair.first);
		bool b_outerShouldDie = false;

		Bullet* b_inner = BulletManager::getBullet(collisionPair.second);
		bool b_innerShouldDie = false;

		if (b_outer == b_inner) {
			continue;
		}
		if (!b_outer->canCollideWith(b_inner)) {
			continue;
		}
		if (CollisionHandler::partiallyCollided(b_outer, b_inner)) {
			InteractionBoolHolder result = EndGameHandler::determineWinner(b_outer, b_inner);
			//if (!b_outerShouldDie) {
				b_outerShouldDie = result.shouldDie; //TODO: does this need to go in the conditional?
			//}
			b_innerShouldDie = result.otherShouldDie;

			if (b_innerShouldDie) {
				bulletDeletionList.push_back(b_inner->getGameID());
			}
		}

		if (b_outerShouldDie) {
			bulletDeletionList.push_back(b_outer->getGameID());
		}
	}

	//clear deaths
	//std::sort(bulletDeletionList.begin(), bulletDeletionList.end());
	for (int i = bulletDeletionList.size() - 1; i >= 0; i--) {
		//BulletManager::deleteBullet(bulletDeletionList[i]);
		BulletManager::deleteBulletByID(bulletDeletionList[i]);
	}

	delete collisionList;
}

void GameMainLoop::bulletToTank() {
	//TODO: tanks and bullets both have powers, so which one gets custom collision priority? (tanks, probably)
	//broad phase
	std::vector<std::pair<int, int>>* collisionList = PhysicsHandler::sweepAndPrune<Circle*, Circle*>(BulletManager::getBulletCollisionList(), TankManager::getTankCollisionList());

	//narrow phase and resolve collision
	std::vector<Game_ID> bulletDeletionList;
	for (int i = 0; i < collisionList->size(); i++) {
		std::pair<int, int> collisionPair = collisionList->at(i);
		Bullet* b = BulletManager::getBullet(collisionPair.first);
		bool shouldBeKilled = false;

		Tank* t = TankManager::getTank(collisionPair.second);
		bool killTank = false;
		bool modifiedTankCollision = false;
		bool overridedTankCollision = false;

		if (b->canCollideWith(t)) {
			for (int k = 0; k < b->bulletPowers.size(); k++) {
				if (b->bulletPowers[k]->modifiesCollisionWithTank) {
					if (b->bulletPowers[k]->modifiedCollisionWithTankCanOnlyWorkIndividually && modifiedTankCollision) {
						continue;
					}
					modifiedTankCollision = true;
					if (b->bulletPowers[k]->overridesCollisionWithTank) {
						overridedTankCollision = true;
					}

					InteractionBoolHolder check_temp = b->bulletPowers[k]->modifiedCollisionWithTank(b, t);
					if (check_temp.shouldDie) {
						shouldBeKilled = true;
					}
					if (check_temp.otherShouldDie) {
						killTank = true;
					}

					if (!b->bulletPowers[k]->modifiedCollisionWithTankCanWorkWithOthers) {
						break;
					}
				}
			}

			if (!overridedTankCollision) {
				if (CollisionHandler::partiallyCollided(t, b)) {
					InteractionBoolHolder result = EndGameHandler::determineWinner(t, b);
					if (result.shouldDie) {
						killTank = true;
					}
					if (result.otherShouldDie) {
						shouldBeKilled = true;
					}
				}
			}
		}

		if (killTank) {
			//TODO: proper implementation?
		}

		if (shouldBeKilled) {
			bulletDeletionList.push_back(b->getGameID());
		}
	}

	//clear deaths
	for (int i = bulletDeletionList.size() - 1; i >= 0; i--) {
		BulletManager::deleteBulletByID(bulletDeletionList[i]);
	}

	delete collisionList;
}

void GameMainLoop::drawMain() const {
	auto start = Diagnostics::getTime();
	Renderer::BeginScene("draw");

	Diagnostics::startTiming("background rect");
	BackgroundRect::draw();
	Diagnostics::endTiming();

	Diagnostics::startTiming("hazards under");
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		HazardManager::getCircleHazard(i)->draw(DrawingLayers::under);
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		HazardManager::getRectHazard(i)->draw(DrawingLayers::under);
	}
	Diagnostics::endTiming();

	Diagnostics::startTiming("level under");
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->draw(DrawingLayers::under);
	}
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->drawLevelEffects(DrawingLayers::under);
	}
	Diagnostics::endTiming();

	Diagnostics::startTiming("powerups");
	for (int i = 0; i < PowerupManager::getNumPowerups(); i++) {
		PowerupManager::getPowerup(i)->draw(DrawingLayers::normal);
	}
	Diagnostics::endTiming();

	Diagnostics::startTiming("hazards");
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		HazardManager::getCircleHazard(i)->draw(DrawingLayers::normal);
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		HazardManager::getRectHazard(i)->draw(DrawingLayers::normal);
	}
	Diagnostics::endTiming();

	Diagnostics::startTiming("walls");
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		WallManager::getWall(i)->draw(DrawingLayers::normal);
	}
	Diagnostics::endTiming();

	Diagnostics::startTiming("bullets");
	for (int i = 0; i < BulletManager::getNumBullets(); i++) {
		BulletManager::getBullet(i)->draw(DrawingLayers::normal);
	}
	Diagnostics::endTiming();

	//drawing text on the GPU will need a library, so names don't get drawn anymore
	/*
	for (int i = 0; i < tanks.size(); i++) {
		tanks[i]->drawName();
	}
	*/

	Diagnostics::startTiming("tanks");
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->draw(DrawingLayers::normal);
	}
	Diagnostics::endTiming();

	Diagnostics::startTiming("hazards effects");
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		HazardManager::getCircleHazard(i)->draw(DrawingLayers::effects);
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		HazardManager::getRectHazard(i)->draw(DrawingLayers::effects);
	}
	Diagnostics::endTiming();

	Diagnostics::startTiming("level effects");
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->draw(DrawingLayers::effects);
	}
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->drawLevelEffects(DrawingLayers::effects);
	}
	Diagnostics::endTiming();

	Diagnostics::startTiming("tanks dead");
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->draw(DrawingLayers::top);
	}
	Diagnostics::endTiming();

	auto end = Diagnostics::getTime();
	Diagnostics::pushGraphTime("upload", Diagnostics::getDiff(start, end));
	Renderer::EndScene();

	const BasicINIParser::BasicINIData& ini_data = GameManager::get_INI();
	if (ini_data.exists("DEBUG", "EnableDebugDrawing") && std::stoi(ini_data.get("DEBUG", "EnableDebugDrawing"))) [[unlikely]] {
		drawLayer(DrawingLayers::debug);
		//TODO: levels should be able to debug draw their starting tank positions (color: white?)
	}

	//end = Diagnostics::getTime();

	//Diagnostics::printPreciseTimings();
	Diagnostics::clearTimes();

	//std::cout << "entire: " << (long long)Diagnostics::getDiff(start, end) << "ms" << std::endl << std::endl;
}

void GameMainLoop::drawLayer(DrawingLayers layer) const {
	//auto start = Diagnostics::getTime();
	Renderer::BeginScene("drawLayer" + std::to_string((int)layer));

	//Diagnostics::startTiming("powerups");
	for (int i = 0; i < PowerupManager::getNumPowerups(); i++) {
		PowerupManager::getPowerup(i)->draw(layer);
	}
	//Diagnostics::endTiming();

	//Diagnostics::startTiming("hazards");
	for (int i = 0; i < HazardManager::getNumCircleHazards(); i++) {
		HazardManager::getCircleHazard(i)->draw(layer);
	}
	for (int i = 0; i < HazardManager::getNumRectHazards(); i++) {
		HazardManager::getRectHazard(i)->draw(layer);
	}
	//Diagnostics::endTiming();

	//Diagnostics::startTiming("walls");
	for (int i = 0; i < WallManager::getNumWalls(); i++) {
		WallManager::getWall(i)->draw(layer);
	}
	//Diagnostics::endTiming();

	//Diagnostics::startTiming("bullets");
	for (int i = 0; i < BulletManager::getNumBullets(); i++) {
		BulletManager::getBullet(i)->draw(layer);
	}
	//Diagnostics::endTiming();

	//Diagnostics::startTiming("tanks");
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->draw(layer);
	}
	//Diagnostics::endTiming();

	//Diagnostics::startTiming("level");
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->draw(layer);
	}
	for (int i = 0; i < LevelManager::getNumLevels(); i++) {
		LevelManager::getLevel(i)->drawLevelEffects(layer);
	}
	//Diagnostics::endTiming();

	Renderer::EndScene();
	//auto end = Diagnostics::getTime();

	//Diagnostics::printPreciseTimings();
	//Diagnostics::clearTimes();

	//std::cout << "entire: " << (long long)Diagnostics::getDiff(start, end) << "ms" << std::endl << std::endl;
}

void GameMainLoop::drawAllLayers() const {
	drawLayer(DrawingLayers::under);
	drawLayer(DrawingLayers::normal);
	drawLayer(DrawingLayers::effects);
	drawLayer(DrawingLayers::top);
	//drawLayer(DrawingLayers::debug);
}
