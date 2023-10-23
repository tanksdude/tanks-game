#pragma once
#include "../level-effect.h"

class InvisibleWallsLevelEffect : public LevelEffect {
	//referred to as "invisible" in JS Tanks (and wasn't technically a level effect)
public:
	virtual std::string getName() const override { return "invisible_walls"; }
	virtual std::vector<std::string> getLevelEffectTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "old" };
		//no random; it must be intentional
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void apply() override;
	virtual void tick(const Level* parent) override;
	virtual void doEffects(Level* parent) const override { return; }

	//virtual void draw() const override; //no need

	InvisibleWallsLevelEffect();
	virtual ~InvisibleWallsLevelEffect();
	static LevelEffect* factory(const GenericFactoryConstructionData&);
};
