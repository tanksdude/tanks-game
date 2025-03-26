#include "ice-level-effect.h"

#include "../tank-manager.h"

std::unordered_map<std::string, float> IceLevelEffect::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", 0.5f });
	weights.insert({ "old", 0.25f });
	return weights;
}

void IceLevelEffect::apply() {
	//TODO: this should not be necessary
	for (int i = 0; i < TankManager::getNumTanks(); i++) {
		TankManager::getTank(i)->acceleration *= iceFactor;
	}
}

IceLevelEffect::IceLevelEffect(float iceAmount) {
	iceFactor = iceAmount;
}

IceLevelEffect::IceLevelEffect() : IceLevelEffect(1.0/8) {}

IceLevelEffect::~IceLevelEffect() {
	//nothing
}

LevelEffect* IceLevelEffect::factory(const GenericFactoryConstructionData& args) {
	if (args.getDataCount() >= 1) [[unlikely]] {
		int count = args.getDataPortionLength(0);

		if (count >= 1) [[likely]] {
			const float* arr = static_cast<const float*>(args.getDataPortion(0).get());
			float ice = arr[0];
			return new IceLevelEffect(ice);
		}
	}

	return new IceLevelEffect();
}
