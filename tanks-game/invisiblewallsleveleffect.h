#pragma once
#include "leveleffect.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class InvisibleWallsLevelEffect : public LevelEffect {
	//referred to as "invisible" in JS Tanks (wasn't a level effect)
public:
	virtual std::string getName() const override { return "invisible_walls"; }
	virtual std::vector<std::string> getLevelEffectTypes() const override;
	virtual std::unordered_map<std::string, float> getWeights() const override;
	virtual void apply() override;
	virtual void tick(const Level* parent) override;
	virtual void doEffects(Level* parent) override;

	//virtual void draw() const override; //no need

	InvisibleWallsLevelEffect();
	virtual ~InvisibleWallsLevelEffect();
	static LevelEffect* factory(int, std::string*);
};
