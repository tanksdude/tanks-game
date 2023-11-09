#pragma once
#include "../level-effect.h"

class DevTinyTanksLevelEffect : public LevelEffect {
public:
	virtual std::string getName() const override { return "tiny_tanks"; }
	virtual std::vector<std::string> getLevelEffectTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev", "random-dev" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void apply() override;
	virtual void tick(const Level* parent) override { return; }
	virtual void doEffects(Level* parent) const override { return; }

	virtual double getTankRadiusMultiplier() const override { return .25; }

	DevTinyTanksLevelEffect();
	virtual ~DevTinyTanksLevelEffect() { return; }
	static LevelEffect* factory(const GenericFactoryConstructionData&);
};
