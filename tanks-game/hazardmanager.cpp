#pragma once
#include "hazardmanager.h"

std::vector<CircleHazard*> HazardManager::circleHazards;
std::vector<RectHazard*> HazardManager::rectHazards;
void HazardManager::initialize() {
	return;
}

CircleHazard* const HazardManager::getCircleHazard(int index) {
	return circleHazards[index];
}

RectHazard* const HazardManager::getRectHazard(int index) {
	return rectHazards[index];
}

void HazardManager::pushCircleHazard(CircleHazard* ch) {
	circleHazards.push_back(ch);
}

void HazardManager::pushRectHazard(RectHazard* rh) {
	rectHazards.push_back(rh);
}

void HazardManager::deleteCircleHazard(int index) {
	delete circleHazards[index];
	circleHazards.erase(circleHazards.begin() + index);
}

void HazardManager::deleteRectHazard(int index) {
	delete rectHazards[index];
	rectHazards.erase(rectHazards.begin() + index);
}