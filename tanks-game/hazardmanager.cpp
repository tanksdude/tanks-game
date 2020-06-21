#include "hazardmanager.h"

std::vector<CircleHazard*> HazardManager::circleHazards;
std::vector<RectHazard*> HazardManager::rectHazards;

std::unordered_map<std::string, CircleHazardFactoryGroup> HazardManager::circleHazardLookup;
std::unordered_map<std::string, RectHazardFactoryGroup> HazardManager::rectHazardLookup;
std::vector<CircleHazardFunction> HazardManager::circleHazardList;
std::vector<RectHazardFunction> HazardManager::rectHazardList;
std::vector<std::string> HazardManager::circleHazardNameList;
std::vector<std::string> HazardManager::rectHazardNameList;

void HazardManager::initialize() {
	return;
}

CircleHazard* HazardManager::getCircleHazard(int index) {
	return circleHazards[index];
}
RectHazard* HazardManager::getRectHazard(int index) {
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

void HazardManager::clearCircleHazards() {
	for (int i = 0; i < circleHazards.size(); i++) {
		delete circleHazards[i];
	}
	circleHazards.clear();
}
void HazardManager::clearRectHazards() {
	for (int i = 0; i < rectHazards.size(); i++) {
		delete rectHazards[i];
	}
	rectHazards.clear();
}


void HazardManager::addCircleHazardFactory(CircleHazardFunction factory) {
	CircleHazard* ch = factory(0, nullptr);
	circleHazardList.push_back(factory);
	circleHazardLookup.insert({ ch->getName(), { factory, ch->getFactoryArgumentCount(), ch->getConstructionType(), ch->getFactoryInformation() } });
	circleHazardNameList.push_back(ch->getName());
	delete ch;
}
void HazardManager::addRectHazardFactory(RectHazardFunction factory) {
	RectHazard* rh = factory(0, nullptr);
	rectHazardList.push_back(factory);
	rectHazardLookup.insert({ rh->getName(), { factory, rh->getFactoryArgumentCount(), rh->getConstructionType(), rh->getFactoryInformation() } });
	rectHazardNameList.push_back(rh->getName());
	delete rh;
}

CircleHazardFunction HazardManager::getCircleHazardFactory(std::string name) {
	return circleHazardLookup[name].getFactory();
}
RectHazardFunction HazardManager::getRectHazardFactory(std::string name) {
	return rectHazardLookup[name].getFactory();
}

CircleHazardFactoryGroup HazardManager::getCircleHazardFactoryGroup(std::string name) {
	return circleHazardLookup[name];
}
RectHazardFactoryGroup HazardManager::getRectHazardFactoryGroup(std::string name) {
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
