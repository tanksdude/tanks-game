#include "hazardmanager.h"

std::vector<CircleHazard*> HazardManager::circleHazards;
std::vector<RectHazard*> HazardManager::rectHazards;

std::unordered_map<std::string, CircleHazardFunction> HazardManager::circleHazardLookup;
std::unordered_map<std::string, RectHazardFunction> HazardManager::rectHazardLookup;
std::vector<CircleHazardFunction> HazardManager::circleHazardList;
std::vector<RectHazardFunction> HazardManager::rectHazardList;
std::vector<std::string> HazardManager::circleHazardNameList;
std::vector<std::string> HazardManager::rectHazardNameList;

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


void HazardManager::addCircleHazardFactory(CircleHazardFunction factory) {
	circleHazardList.push_back(factory);
	CircleHazard* ch = factory(0,nullptr);
	circleHazardLookup.insert({ ch->getName(), factory });
	circleHazardNameList.push_back(ch->getName());
	delete ch;
}
void HazardManager::addRectHazardFactory(RectHazardFunction factory) {
	rectHazardList.push_back(factory);
	RectHazard* rh = factory(0,nullptr);
	rectHazardLookup.insert({ rh->getName(), factory });
	rectHazardNameList.push_back(rh->getName());
	delete rh;
}

CircleHazardFunction HazardManager::getCircleHazardFactory(std::string name) {
	return circleHazardLookup[name];
}
RectHazardFunction HazardManager::getRectHazardFactory(std::string name) {
	return rectHazardLookup[name];
}

std::string HazardManager::getCircleHazardName(int index) {
	return circleHazardNameList[index];
}
std::string HazardManager::getRectHazardName(int index) {
	return rectHazardNameList[index];
}

int HazardManager::getNumCircleHazardTypes() {
	return circleHazardNameList.size();
}
int HazardManager::getNumRectHazardTypes() {
	return rectHazardNameList.size();
}
