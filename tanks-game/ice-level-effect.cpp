#include "ice-level-effect.h"
#include "tank-manager.h"

std::unordered_map<std::string, float> IceLevelEffect::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", .5f });
	weights.insert({ "old", .25f });
	return weights;
}

void IceLevelEffect::apply() {
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->acceleration *= iceFactor;
		//TODO: should tanks reset themselves if it's the first stage?
	}
}

void IceLevelEffect::tick(const Level* parent) {
	//nothing
}

void IceLevelEffect::doEffects(Level* parent) {
	//nothing
}

IceLevelEffect::IceLevelEffect(double iceAmount) {
	iceFactor = iceAmount;
}

IceLevelEffect::IceLevelEffect() : IceLevelEffect(1.0/8) {}

IceLevelEffect::~IceLevelEffect() {
	//nothing
}

LevelEffect* IceLevelEffect::factory(GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) {
		int count = args.getDataPortionLength(0);

		if (count >= 1) {
			double* arr = (double*)(args.getDataPortion(0));
			double ice = arr[0];
			return new IceLevelEffect(ice);
		}
	}
	return new IceLevelEffect();
}
