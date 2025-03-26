#pragma once
#include "../level-effect.h"

class IceLevelEffect : public LevelEffect {
protected:
	float iceFactor;

public:
	virtual std::string getName() const override { return "ice"; }
	virtual std::vector<std::string> getLevelEffectTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla-extra", "old" };
		//no random; it's hated
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void apply() override;
	virtual void tick(const Level* parent) override { return; }
	virtual void doEffects(Level* parent) const override { return; }

	virtual float getTankAccelerationMultiplier() const override { return iceFactor; }

	IceLevelEffect();
	IceLevelEffect(float iceFactor);
	virtual ~IceLevelEffect();
	static LevelEffect* factory(const GenericFactoryConstructionData&);
};
