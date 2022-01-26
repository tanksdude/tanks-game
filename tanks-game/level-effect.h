#pragma once
class LevelEffect;

#include "drawable-thing.h"
#include "level.h"
#include <vector>
#include <string>
#include <unordered_map>
#include "generic-factory-construction-data.h"

//TODO: level effects basically need to be powers but for levels (needs to be able to override some things in GameMainLoop, I think)
class LevelEffect : public DrawableThing {
public:
	virtual std::string getName() const = 0;
	virtual std::vector<std::string> getLevelEffectTypes() const = 0; //pure virtual to make sure level effects define it
	virtual std::unordered_map<std::string, float> getWeights() const = 0; //intended range: (0,1]

public:
	virtual void apply() = 0;
	virtual void tick(const Level* parent) = 0; //update variables and stuff
	virtual void doEffects(Level* parent) = 0; //do what needs to be done

	virtual double getTankMaxSpeedMultiplier() const { return 1; }
	virtual double getTankAccelerationMultiplier() const { return 1; }
	virtual double getTankRadiusMultiplier() const { return 1; }
	virtual double getTankFiringRateMultiplier() const { return 1; }
	virtual double getTankTurningIncrementMultiplier() const { return 1; }
	//they all stack because you shouldn't have too many

	virtual void draw() const override { return; }
	virtual void draw(DrawingLayers) const override { return; }
	virtual void poseDraw() const override { return; }
	virtual void poseDraw(DrawingLayers) const override { return; }
	virtual void ghostDraw(float alpha) const override { return; }
	virtual void ghostDraw(DrawingLayers, float alpha) const override { return; }

	virtual ~LevelEffect() { return; }
	static LevelEffect* factory(GenericFactoryConstructionData&);
};
