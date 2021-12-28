#include "bullet-manager.h"

std::vector<Bullet*> BulletManager::bullets;
int BulletManager::maxBullets = 2048;
#if _DEBUG
bool BulletManager::limitBullets = false;
#else
bool BulletManager::limitBullets = true;
#endif
bool BulletManager::autoLimitBullets = false;

void BulletManager::initialize() {
	bullets.reserve(4096);
	//512 is the max I expect to be fired at once (3 multishots); 2048 might get reached, so 4096 should be way more than enough
}

Bullet* BulletManager::getBullet(int index) {
	return bullets[index];
}

Bullet* BulletManager::getBulletByID(Game_ID gameID) {
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->getGameID() == gameID) {
			return bullets[i];
		}
	}
	return nullptr;
}

void BulletManager::pushBullet(Bullet* b) {
	bullets.push_back(b);
	if (autoLimitBullets && limitBullets) {
		while (bullets.size() > maxBullets) {
			deleteBullet(0);
		}
	}
}

void BulletManager::forceLimitBullets() {
	if (limitBullets) {
		while (bullets.size() > maxBullets) {
			deleteBullet(0);
		}
	}
}

void BulletManager::deleteBullet(int index) {
	delete bullets[index];
	bullets.erase(bullets.begin() + index);
}

void BulletManager::deleteBulletByID(Game_ID gameID) {
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->getGameID() == gameID) {
			deleteBullet(i);
			break;
		}
	}
}

void BulletManager::clearBullets() {
	for (int i = 0; i < bullets.size(); i++) {
		delete bullets[i];
	}
	bullets.clear();
}

std::vector<Circle*> BulletManager::getBulletCollisionList() {
	return std::vector<Circle*>(bullets.begin(), bullets.end());
}