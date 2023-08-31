#pragma once
#include "bullet.h"
#include <vector>

class BulletManager {
	friend class ResetThings;

private:
	static std::vector<Bullet*> bullets;
	static int maxBullets;
	static bool limitBullets;
	static bool autoLimitBullets; //limit bullets when they're first added
	static void clearBullets(); //for ResetThings

public:
	static void initialize();
	static Bullet* getBullet(int index);
	static Bullet* getBulletByID(Game_ID);
	static void pushBullet(Bullet*);
	static int getNumBullets() { return bullets.size(); }
	static void deleteBullet(int index);
	static void deleteBulletByID(Game_ID);
	static void forceLimitBullets();

	static std::vector<Circle*> getBulletCollisionList();

private:
	BulletManager() = delete;
	BulletManager(const BulletManager&) = delete;
};
