#include "bulletmanager.h"

std::vector<Bullet*> BulletManager::bullets;
int BulletManager::maxBullets = 2048;
bool BulletManager::limitBullets = false;

void BulletManager::initialize() {
	bullets.reserve(4096);
	//512 is the max I expect to be fired at once (3 multishots); 2048 might get reached, so 4096 should be way more than enough
}

Bullet* BulletManager::getBullet(int index) {
	return bullets[index];
}

void BulletManager::pushBullet(Bullet* b) {
	bullets.push_back(b);
	if (limitBullets) {
		while (bullets.size() > maxBullets) {
			bullets.erase(bullets.begin() + 0);
		}
	}
}

void BulletManager::deleteBullet(int index) {
	delete bullets[index];
	bullets.erase(bullets.begin() + index);
}

void BulletManager::clearBullets() {
	for (int i = 0; i < bullets.size(); i++) {
		delete bullets[i];
	}
	bullets.clear();
}
