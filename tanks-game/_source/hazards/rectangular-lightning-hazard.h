#pragma once
#include "../rect-hazard.h"
#include "../generalized-lightning.h"

#include "../constants.h"

class RectangularLightningHazard : public RectHazard, public GeneralizedLightning {
	//called LightningZone in JS Tanks
protected:
	//unsigned int maxBolts; // = 1;
	virtual void refreshBolt(LightningBolt*) const override;
	virtual void refreshBolt(LightningBolt*, double smaller, double larger) const;
	virtual void pushBolt(LightningBolt*) override;
	virtual void pushDefaultBolt(int num, bool randomize) override; //randomize should be true all of the time

	[[nodiscard]] inline Circle* getCenterPoint() const; //for checks when a bullet/tank collides (needs to be a function in case the lightning changes size or position)

public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "old", "random-old", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual RectHazardCollisionType getCollisionType() const override { return RectHazardCollisionType::under; }

	virtual bool actuallyCollided(const Tank*) const override { return currentlyActive; }
	//bool modifiesTankCollision = true;
	virtual void modifiedTankCollision(Tank*) override { return; }
	//bool hasSpecialEffectTankCollision = true;
	virtual void specialEffectTankCollision(const Tank*) override;

	virtual bool actuallyCollided(const Bullet*) const override { return currentlyActive; }
	//bool modifiesBulletCollision = true;
	virtual void modifiedBulletCollision(Bullet*) override { return; }
	//bool hasSpecialEffectBulletCollision = true;
	virtual void specialEffectBulletCollision(const Bullet*) override;
protected:
	virtual void specialEffectCircleCollision(const Circle*); //tanks and bullets are both circles, so calculating the bolt positions would be the same

protected:
	virtual float getDefaultOffense() const override { return .5; } //1.5?
	virtual float getDefaultDefense() const override { return HIGH_TIER; }

public:
	virtual bool validLocation() const override;
	virtual bool reasonableLocation() const override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "lightning"; }

	virtual void tick() override { GeneralizedLightning::tick(); }
	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

protected:
	virtual inline void drawBackground(bool pose, float alpha = 1.0f) const;
	virtual inline void drawBackgroundOutline(float alpha) const; //called by drawBackground()
	virtual inline void drawBolts(float alpha = 1.0f) const;
	virtual inline void drawBolts_Pose(float alpha = 1.0f) const;

public:
	RectangularLightningHazard(double xpos, double ypos, double width, double height);
	virtual ~RectangularLightningHazard();
	static RectHazard* factory(const GenericFactoryConstructionData&);
	static RectHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
	virtual int getFactoryArgumentCount() const override { return 4; }
	virtual RectHazardConstructionTypes getConstructionType() const override { return RectHazardConstructionTypes::standardConstruction; }
	virtual RectFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
