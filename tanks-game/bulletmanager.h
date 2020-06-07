#pragma once
#include "bullet.h"
#include <vector>

class BulletManager {
	friend class ResetThings;
private:
	static std::vector<Bullet*> bullets;
	static int maxBullets; //currently not used
	static void clearBullets(); //for ResetThings
public:
	static void initialize();
	static Bullet* const getBullet(int index);
	//T* const f() means the pointer is constant but the inside stuff can be modified
	//no actual point to this according to https://stackoverflow.com/questions/3888470/c-const-member-function-that-returns-a-const-pointer-but-what-type-of-const
	static void pushBullet(Bullet*);
	static int getNumBullets() { return bullets.size(); }
	static void deleteBullet(int index);
};
