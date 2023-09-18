#pragma once
#include "level-effect.h"

#include "bullet.h"

class MinefieldLevelEffect : public LevelEffect {
	//called "mines" in JS Tanks
protected:
	double tickCount;
	double tickCycle;

	int maxNumOfMines;
	double minefield_startX;
	double minefield_startY;
	double minefield_areaWidth;
	double minefield_areaHeight;

	int initialMineCount;
	Bullet* ghostMine;
	virtual Bullet* genMine() const;
	virtual inline void pushRandomMine() const;

public:
	virtual std::string getName() const override { return "minefield"; }
	virtual std::vector<std::string> getLevelEffectTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old" };
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

	MinefieldLevelEffect();
	MinefieldLevelEffect(double x_start, double y_start, double area_width, double area_height);
	MinefieldLevelEffect(double x_start, double y_start, double area_width, double area_height, int initialMineCount);
	virtual ~MinefieldLevelEffect();
	static LevelEffect* factory(const GenericFactoryConstructionData&);
};
