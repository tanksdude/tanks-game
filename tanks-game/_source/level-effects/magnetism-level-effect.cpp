#include "magnetism-level-effect.h"

#include "../tank-manager.h"
#include "../powerup-manager.h"

std::unordered_map<std::string, float> MagnetismLevelEffect::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 0.5f }); //powerups need to be manually watched when not watching everything
	return weights;
}

void MagnetismLevelEffect::watchPowerSquare(Game_ID p) {
	bool found = false;
	for (int i = 0; i < watching.size(); i++) {
		if (watching[i] == p) {
			found = true;
			std::cerr << "WARNING: MagnetismLevelEffect is already watching PowerSquare ID#" << p << std::endl;
			break;
		}
	}
	if (!found) {
		watching.push_back(p);
	}
}

void MagnetismLevelEffect::watchLastPowerSquaresPushed(int count) {
	for (int i = count; i > 0; i--) {
		watchPowerSquare(PowerupManager::getPowerup(PowerupManager::getNumPowerups()-i)->getGameID());
	}
}

void MagnetismLevelEffect::unwatchPowerSquare(Game_ID powerupID) {
	for (int i = 0; i < watching.size(); i++) {
		if (watching[i] == powerupID) {
			watching.erase(watching.begin() + i);
			break;
		}
	}
}

void MagnetismLevelEffect::apply() {
	if (watchAllPowerups) {
		for (int i = 0; i < PowerupManager::getNumPowerups(); i++) {
			watchPowerSquare(PowerupManager::getPowerup(i)->getGameID());
		}
	}
}

void MagnetismLevelEffect::tick(const Level* parent) {
	for (int i = 0; i < watching.size(); i++) {
		PowerSquare* p = PowerupManager::getPowerupByID(watching[i]);
		if (p == nullptr) {
			//for now, delete from the list if they can't be found, because this can't refresh the Game_ID of a respawned powerup
			watching.erase(watching.begin() + i);
			i--;
			continue;
		}
	}
}

void MagnetismLevelEffect::doEffects(Level* parent) const {
	//an option would be to check all powerups, and if there are new ones, watch them
	//note: doesn't work well with respawning powerups because those powersquares get different Game_IDs
	//a fix would probably involve making PowerSquareWatcher a regular element of the game

	for (int i = 0; i < watching.size(); i++) {
		PowerSquare* p = PowerupManager::getPowerupByID(watching[i]);
		if (p != nullptr) [[likely]] {
			const double p_x = p->x + p->w/2;
			const double p_y = p->y + p->h/2;
			double moveDelta[2] = { 0, 0 };
			for (int j = 0; j < TankManager::getNumTanks(); j++) {
				const Tank* t = TankManager::getTank(j);
				const SimpleVector2D distToTank = SimpleVector2D(t->x - p_x, t->y - p_y);
				if (distToTank.getMagnitude() < distToStartMoving) {
					const double maxMovePercentage = 1 - (distToTank.getMagnitude() / distToStartMoving); //linear interpolation
					moveDelta[0] += (maxMoveAmount * maxMovePercentage) * cos(distToTank.getAngle());
					moveDelta[1] += (maxMoveAmount * maxMovePercentage) * sin(distToTank.getAngle());
				}
			}
			p->x += moveDelta[0];
			p->y += moveDelta[1];
		} else {
			//very unlikely to get here; can only happen if a later level effect's tick() altered powerups after this's tick()
		}
	}
}

MagnetismLevelEffect::MagnetismLevelEffect(bool watchEverything, double distToStartMoving, double maxMagnetismStrength) {
	this->watchAllPowerups = watchEverything;
	this->distToStartMoving = distToStartMoving;
	this->maxMoveAmount = maxMagnetismStrength;
}

MagnetismLevelEffect::MagnetismLevelEffect(bool watchEverything) : MagnetismLevelEffect(watchEverything, 96, 1) {}

MagnetismLevelEffect::MagnetismLevelEffect() : MagnetismLevelEffect(true) {}

MagnetismLevelEffect::~MagnetismLevelEffect() {
	//nothing
}

LevelEffect* MagnetismLevelEffect::factory(const GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) [[likely]] {
		int count = args.getDataPortionLength(0);

		if (count >= 1) [[likely]] {
			const bool* arr = static_cast<const bool*>(args.getDataPortion(0).get());
			bool watch = arr[0];

			if (args.getDataCount() >= 2) {
				int count_data = args.getDataPortionLength(1);
				if (count_data >= 2) {
					const double* arr_data = static_cast<const double*>(args.getDataPortion(1).get());
					double dist = arr_data[0];
					double strength = arr_data[1];
					return new MagnetismLevelEffect(watch, dist, strength);
				}
			}
			return new MagnetismLevelEffect(watch);
		}
	}
	return new MagnetismLevelEffect();
}
