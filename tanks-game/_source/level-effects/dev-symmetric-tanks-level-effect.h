#pragma once
#include "../level-effect.h"

class DevSymmetricTanksLevelEffect : public LevelEffect {
protected:
	bool mirrorAngle;

public:
	virtual std::string getName() const override { return "symmetric_tanks"; }
	virtual std::vector<std::string> getLevelEffectTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "dev" }; //no random-dev
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void apply() override { return; }
	virtual void tick(const Level* parent) override;
	virtual void doEffects(Level* parent) const override { return; }

	DevSymmetricTanksLevelEffect();
	DevSymmetricTanksLevelEffect(bool alsoDoAngle);
	virtual ~DevSymmetricTanksLevelEffect() { return; }
	static LevelEffect* factory(const GenericFactoryConstructionData&);
};
