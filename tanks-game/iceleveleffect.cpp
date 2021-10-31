#include "iceleveleffect.h"
#include "tankmanager.h"

std::vector<std::string> IceLevelEffect::getLevelEffectTypes() const {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "old" };
	return types;
}

std::unordered_map<std::string, float> IceLevelEffect::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla-extra", .25f });
	weights.insert({ "old", .125f });
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

LevelEffect* IceLevelEffect::factory(int argc, std::string* argv) {
	if (argc >= 1) {
		double ice = std::stoi(argv[0]);
		return new IceLevelEffect(ice);
	}
	return new IceLevelEffect();
}
