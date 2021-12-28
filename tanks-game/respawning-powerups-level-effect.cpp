#include "respawning-powerups-level-effect.h"
#include "powerup-manager.h"

std::unordered_map<std::string, float> RespawningPowerupsLevelEffect::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 2.0f });
	weights.insert({ "random-vanilla", 1.0f }); //powerups need to be manually watched when not watching everything
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
		const PowerSquare* p = PowerupManager::getPowerupByID(this->powerupID);
		if (p == nullptr) {
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
		powerupCopy->ghostDraw(tickCount / maxTickCount); //TODO: drawing layer things?
	}
}

void RespawningPowerupsLevelEffect::PowerSquareWatcher::ghostDraw(float alpha) const {
	powerupCopy->ghostDraw(alpha); //TODO: drawing layer things?
}

RespawningPowerupsLevelEffect::PowerSquareWatcher::PowerSquareWatcher(const PowerSquare* p) :
	RespawningPowerupsLevelEffect::PowerSquareWatcher::PowerSquareWatcher(p, 500) {}
//500 or 1000? dunno

RespawningPowerupsLevelEffect::PowerSquareWatcher::~PowerSquareWatcher() {
	delete powerupCopy;
}

void RespawningPowerupsLevelEffect::watchPowerSquare(const PowerSquare* p) {
	bool found = false;
	for (int i = 0; i < watching.size(); i++) {
		if (watching[i]->getPowerupID() == p->getGameID()) {
			found = true;
			std::cerr << "WARNING: RespawningPowerupsLevelEffect is already watching PowerSquare ID\#" << p->getGameID() << std::endl;
			break;
		}
	}
	if (!found) {
		watching.push_back(new PowerSquareWatcher(p));
	}
}

void RespawningPowerupsLevelEffect::unwatchPowerSquare(Game_ID powerupID) {
	for (int i = 0; i < watching.size(); i++) {
		if (watching[i]->getPowerupID() == powerupID) {
			watching.erase(watching.begin() + i);
			break;
		}
	}
}

void RespawningPowerupsLevelEffect::apply() {
	if (watchAllPowerups) {
		for (int i = 0; i < PowerupManager::getNumPowerups(); i++) {
			watchPowerSquare(PowerupManager::getPowerup(i));
		}
	}
}

void RespawningPowerupsLevelEffect::tick(const Level* parent) {
	//an option would be to check all powerups, and if there are new ones, watch them
	//this isn't done because of dev menu inserting

	for (int i = 0; i < watching.size(); i++) {
		watching[i]->tick();
	}
}

void RespawningPowerupsLevelEffect::doEffects(Level* parent) {
	//nothing
}

void RespawningPowerupsLevelEffect::draw() const {
	for (int i = 0; i < watching.size(); i++) {
		watching[i]->draw();
	}
}

void RespawningPowerupsLevelEffect::draw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		case DrawingLayers::normal:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for RespawningPowerupsLevelEffect::draw!" << std::endl;
		case DrawingLayers::effects:
			draw();
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void RespawningPowerupsLevelEffect::poseDraw() const {
	//TODO
	ghostDraw(.5f);
}

void RespawningPowerupsLevelEffect::poseDraw(DrawingLayers layer) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		case DrawingLayers::normal:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for RespawningPowerupsLevelEffect::poseDraw!" << std::endl;
		case DrawingLayers::effects:
			poseDraw();
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

void RespawningPowerupsLevelEffect::ghostDraw(float alpha) const {
	for (int i = 0; i < watching.size(); i++) {
		watching[i]->ghostDraw(alpha);
	}
}

void RespawningPowerupsLevelEffect::ghostDraw(DrawingLayers layer, float alpha) const {
	switch (layer) {
		case DrawingLayers::under:
			//nothing
			break;

		case DrawingLayers::normal:
			//nothing
			break;

		default:
			std::cerr << "WARNING: unknown DrawingLayer for RespawningPowerupsLevelEffect::ghostDraw!" << std::endl;
		case DrawingLayers::effects:
			ghostDraw(alpha);
			break;

		case DrawingLayers::top:
			//nothing
			break;

		case DrawingLayers::debug:
			//later
			break;
	}
}

RespawningPowerupsLevelEffect::RespawningPowerupsLevelEffect(bool watchEverything) {
	watchAllPowerups = watchEverything;
}

RespawningPowerupsLevelEffect::RespawningPowerupsLevelEffect() : RespawningPowerupsLevelEffect(true) {}

RespawningPowerupsLevelEffect::~RespawningPowerupsLevelEffect() {
	for (int i = 0; i < watching.size(); i++) {
		delete watching[i];
	}
	watching.clear(); //not really necessary
}

LevelEffect* RespawningPowerupsLevelEffect::factory(int argc, std::string* argv) {
	if (argc >= 1) {
		//don't push the powerups to watch at the start because level construction happens before level initialization
		bool watch = std::stoi(argv[0]);
		return new RespawningPowerupsLevelEffect(watch);
	}
	return new RespawningPowerupsLevelEffect();
}