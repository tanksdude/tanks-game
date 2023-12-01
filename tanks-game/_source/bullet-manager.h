#pragma once
#include <vector>
#include "bullet.h"

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
	static Bullet* getBullet(unsigned int index);
	static Bullet* getBulletByID(Game_ID);
	static void pushBullet(Bullet*);
	static unsigned int getNumBullets() { return bullets.size(); }
	static void deleteBullet(unsigned int index);
	static void deleteBulletByID(Game_ID);
	static void forceLimitBullets();

	static std::vector<Circle*> getBulletCollisionList();

private:
	BulletManager() = delete;
	BulletManager(const BulletManager&) = delete;
};
