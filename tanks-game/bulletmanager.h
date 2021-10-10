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
	static void pushBullet(Bullet*);
	static int getNumBullets() { return bullets.size(); }
	static void deleteBullet(int index);
	static void forceLimitBullets();

private:
	BulletManager() {}
	BulletManager(const BulletManager&) {}
};
