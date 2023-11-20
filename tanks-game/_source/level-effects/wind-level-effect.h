#pragma once
#include "../level-effect.h"

#include "../simple-vector-2d.h"

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
	static SimpleVector2D vertices_spike[3];
	static SimpleVector2D vertices_arrow[7];
	static unsigned int indices_spike[1*3];
	static unsigned int indices_arrow[3*3];
	static bool initialized_vertices;

	static bool initializeVertices();

public:
	virtual std::string getName() const override { return "wind"; }
	virtual std::vector<std::string> getLevelEffectTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual void apply() override { return; }
	virtual void tick(const Level* parent) override;
	virtual void doEffects(Level* parent) const override;

	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

	WindLevelEffect();
	WindLevelEffect(bool transitionWind);
	virtual ~WindLevelEffect();
	static LevelEffect* factory(const GenericFactoryConstructionData&);
};
