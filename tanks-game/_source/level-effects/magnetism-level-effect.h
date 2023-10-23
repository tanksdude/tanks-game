#pragma once
#include "../level-effect.h"

#include <vector>
#include "../power-square.h"

class MagnetismLevelEffect : public LevelEffect {
protected:
	bool watchAllPowerups;
	double distToStartMoving; //TODO: should each powerup have these two parameters customizable?
	double maxMoveAmount; //theoretically, this could be negative...
	std::vector<Game_ID> watching;

public:
	unsigned int getNumWatching() const { return watching.size(); }
	virtual void watchPowerSquare(Game_ID);
	virtual void watchLastPowerSquaresPushed(int count);
	virtual void unwatchPowerSquare(Game_ID powerupID);

public:
	virtual std::string getName() const override { return "magnetism"; }
	virtual std::vector<std::string> getLevelEffectTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void apply() override;
	virtual void tick(const Level* parent) override;
	virtual void doEffects(Level* parent) const override;

	MagnetismLevelEffect();
	MagnetismLevelEffect(bool watchEverything);
	MagnetismLevelEffect(bool watchEverything, double distToStartMoving, double maxMagnetismStrength);
	virtual ~MagnetismLevelEffect();
	static LevelEffect* factory(const GenericFactoryConstructionData&);
};
