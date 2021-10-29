#include "respawningpowerupsleveleffect.h"
#include "powerupmanager.h"
#include "colormixer.h"
#include "backgroundrect.h"
#include "renderer.h"

std::vector<std::string> RespawningPowerupsLevelEffect::getLevelEffectTypes() const {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla"/*, "random-vanilla"*/ };
	return types;
}

std::unordered_map<std::string, float> RespawningPowerupsLevelEffect::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	//weights.insert({ "random-vanilla", .5f }); //can't because the powerups need to be manually watched
	return weights;
}

void RespawningPowerupsLevelEffect::PowerSquareWatcher::tick() {
	if (this->isGone) {
		tickCount++;
		if (tickCount >= maxTickCount) {
			//new PowerSquare needed
			PowerSquare* p = new PowerSquare(*powerupCopy); //copy by reference (copy constructor)
			PowerupManager::pushPowerup(p);
			reset(p);
		}
	} else {
		bool found = false;
		for (int i = 0; i < PowerupManager::getNumPowerups(); i++) {
			const PowerSquare* p = PowerupManager::getPowerup(i);
			if (p->getGameID() == this->powerupID) {
				found = true;
				break;
			}
		}
		if (!found) {
			setIsGone();
		}
	}
}

void RespawningPowerupsLevelEffect::PowerSquareWatcher::reset(const PowerSquare* p) {
	this->powerupID = p->getGameID();
	this->tickCount = 0;
	this->isGone = false;
}

RespawningPowerupsLevelEffect::PowerSquareWatcher::PowerSquareWatcher(const PowerSquare* p, double count) {
	this->powerupID = p->getGameID();
	this->tickCount = 0;
	this->maxTickCount = count;
	this->isGone = false;
	this->powerupCopy = new PowerSquare(*p); //copy by reference (copy constructor)
}

void RespawningPowerupsLevelEffect::PowerSquareWatcher::draw() const {
	if (isGone) {
		powerupCopy->ghostDraw(tickCount / maxTickCount);
	}
}

RespawningPowerupsLevelEffect::PowerSquareWatcher::PowerSquareWatcher(const PowerSquare* p) :
	RespawningPowerupsLevelEffect::PowerSquareWatcher::PowerSquareWatcher(p, 500) {}
//500 or 1000? dunno

RespawningPowerupsLevelEffect::PowerSquareWatcher::~PowerSquareWatcher() {
	delete powerupCopy;
}

void RespawningPowerupsLevelEffect::watchPowerSquare(const PowerSquare* p) {
	watching.push_back(new PowerSquareWatcher(p));
}

void RespawningPowerupsLevelEffect::unwatchPowerSquare(Game_ID powerupID) {
	for (int i = 0; i < watching.size(); i++) {
		if (watching[i]->getPowerupID() == powerupID) {
			watching.erase(watching.begin() + i);
			break; //one unique gameID per object
		}
	}
}

void RespawningPowerupsLevelEffect::apply() {
	//nothing
}

void RespawningPowerupsLevelEffect::tick(const Level* parent) {
	for (int i = 0; i < watching.size(); i++) {
		watching[i]->tick();
	}
}

void RespawningPowerupsLevelEffect::doEffects(Level* parent) {
	//nothing
}

void RespawningPowerupsLevelEffect::draw() const {
	//TODO: draw the ghost powerups

	for (int i = 0; i < watching.size(); i++) {
		watching[i]->draw();
	}
}

RespawningPowerupsLevelEffect::RespawningPowerupsLevelEffect() {
	//nothing
}

RespawningPowerupsLevelEffect::~RespawningPowerupsLevelEffect() {
	for (int i = 0; i < watching.size(); i++) {
		delete watching[i];
	}
	watching.clear(); //not really necessary
}

LevelEffect* RespawningPowerupsLevelEffect::factory(int argc, std::string* argv) {
	if (argc >= 1) {
		//don't push the powerups to watch at the start because level construction happens before level initialization
	}
	return new RespawningPowerupsLevelEffect();
}
