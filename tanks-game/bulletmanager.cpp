#pragma once
#include "bulletmanager.h"

std::vector<Bullet*> BulletManager::bullets;
void BulletManager::initialize() {
	bullets.reserve(800);
}

Bullet* const BulletManager::getBullet(int index) {
	return bullets[index];
}

void BulletManager::pushBullet(Bullet* b) {
	bullets.push_back(b);
}

void BulletManager::deleteBullet(int index) {
	delete bullets[index];
	bullets.erase(bullets.begin() + index);
}