#pragma once
#include "leveleffect.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class IceLevelEffect : public LevelEffect {
protected:
	double iceFactor;

public:
	virtual std::string getName() const override { return "ice"; }
	virtual std::vector<std::string> getLevelEffectTypes() const override;
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual void apply() override;
	virtual void tick(const Level* parent) override;
	virtual void doEffects(Level* parent) override;

	virtual double getTankAccelerationMultiplier() const { return iceFactor; }

	IceLevelEffect();
	IceLevelEffect(double iceFactor);
	virtual ~IceLevelEffect();
	static LevelEffect* factory(int, std::string*);
};
