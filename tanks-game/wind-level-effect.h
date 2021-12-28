#pragma once
#include "level-effect.h"
#include "simple-vector-2d.h"

#include "vertex-array.h"
#include "vertex-buffer.h"
#include "index-buffer.h"

class WindLevelEffect : public LevelEffect {
protected:
	double tickCount;
	double tickCycle;
	unsigned int currentState;
	unsigned int maxState;
	double* stateMultiplier;
	SimpleVector2D pushDirection;

	virtual double getWindStrengthMultiplier() const;

private:
	static VertexArray* va; //spike
	static VertexBuffer* vb;
	static IndexBuffer* ib;
	static VertexArray* va_extra; //arrow
	static VertexBuffer* vb_extra;
	static IndexBuffer* ib_extra;
	static bool initialized_GPU;

	static bool initializeGPU();
	static bool uninitializeGPU();

public:
	virtual std::string getName() const override { return "wind"; }
	virtual std::vector<std::string> getLevelEffectTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void apply() override;
	virtual void tick(const Level* parent) override;
	virtual void doEffects(Level* parent) override;

	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

	WindLevelEffect();
	WindLevelEffect(bool transitionWind);
	virtual ~WindLevelEffect();
	static LevelEffect* factory(int, std::string*);
};