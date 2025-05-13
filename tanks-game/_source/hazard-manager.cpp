#include "hazard-manager.h"

#include "game-manager.h"
#include "hazard-data-governor.h"

std::vector<CircleHazard*> HazardManager::circleHazards;
std::vector<RectHazard*> HazardManager::rectHazards;

void HazardManager::initialize() {
	return;
}

CircleHazard* HazardManager::getCircleHazard(unsigned int index) {
	return circleHazards[index];
}
RectHazard* HazardManager::getRectHazard(unsigned int index) {
	return rectHazards[index];
}

CircleHazard* HazardManager::getCircleHazardByID(Game_ID gameID) {
	for (int i = 0; i < circleHazards.size(); i++) {
		if (circleHazards[i]->getGameID() == gameID) {
			return circleHazards[i];
		}
	}
	return nullptr;
}
RectHazard* HazardManager::getRectHazardByID(Game_ID gameID) {
	for (int i = 0; i < rectHazards.size(); i++) {
		if (rectHazards[i]->getGameID() == gameID) {
			return rectHazards[i];
		}
	}
	return nullptr;
}

void HazardManager::pushCircleHazard(std::string type, std::string name, const GenericFactoryConstructionData& data) {
	CircleHazard* ch = makeCircleHazard(type, name, data);
	pushCircleHazard(ch);
	GameManager::pushObject(ch);
}
void HazardManager::pushRectHazard(std::string type, std::string name, const GenericFactoryConstructionData& data) {
	RectHazard* rh = makeRectHazard(type, name, data);
	pushRectHazard(rh);
	GameManager::pushObject(rh);
}

void HazardManager::deleteCircleHazard(unsigned int index) {
	CircleHazard* ch = circleHazards[index];
	GameManager::deleteObjectByID(ch->getGameID());
	delete circleHazards[index];
	circleHazards.erase(circleHazards.begin() + index);
}
void HazardManager::deleteRectHazard(unsigned int index) {
	RectHazard* rh = rectHazards[index];
	GameManager::deleteObjectByID(rh->getGameID());
	delete rectHazards[index];
	rectHazards.erase(rectHazards.begin() + index);
}

void HazardManager::deleteCircleHazardByID(Game_ID gameID) {
	GameManager::deleteObjectByID(gameID);
	for (int i = 0; i < circleHazards.size(); i++) {
		if (circleHazards[i]->getGameID() == gameID) {
			deleteCircleHazard(i);
			break;
		}
	}
}
void HazardManager::deleteRectHazardByID(Game_ID gameID) {
	GameManager::deleteObjectByID(gameID);
	for (int i = 0; i < rectHazards.size(); i++) {
		if (rectHazards[i]->getGameID() == gameID) {
			deleteRectHazard(i);
			break;
		}
	}
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

CircleHazard* HazardManager::makeCircleHazard(std::string type, std::string name, const GenericFactoryConstructionData& data) {
	return HazardDataGovernor::getCircleHazardFactory(type, name)(data);
}
RectHazard* HazardManager::makeRectHazard(std::string type, std::string name, const GenericFactoryConstructionData& data) {
	return HazardDataGovernor::getRectHazardFactory(type, name)(data);
}

void HazardManager::pushCircleHazard(CircleHazard* ch) {
	circleHazards.push_back(ch);
	GameManager::pushObject(ch);
	ch->initialize();
}
void HazardManager::pushRectHazard(RectHazard* rh) {
	rectHazards.push_back(rh);
	GameManager::pushObject(rh);
	rh->initialize();
}

std::vector<Circle*> HazardManager::getCircleHazardCollisionList() {
	return std::vector<Circle*>(circleHazards.begin(), circleHazards.end());
}
std::vector<Rect*> HazardManager::getRectHazardCollisionList() {
	return std::vector<Rect*>(rectHazards.begin(), rectHazards.end());
}
