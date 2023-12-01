#pragma once
#include "rectangular-lightning-hazard.h"

class HorizontalLightningHazard : public RectangularLightningHazard {
	//just called Lightning in JS Tanks
protected:
	//bool flexible; //worry about later
	//unsigned int maxBolts; // = 2;
	virtual void refreshBolt(LightningBolt*) const override; //uses RectangularLightning::refreshBolt
	virtual void simpleRefreshBolt(LightningBolt*) const; //fast path for refreshing a bolt that goes from beginning to end
	virtual void pushBolt(LightningBolt* l) override { pushBolt(l, false); }
	virtual void pushBolt(LightningBolt*, bool simpleRefresh);
	virtual void pushDefaultBolt(int num, bool randomize) override;

	[[nodiscard]] inline Circle* getLeftPoint() const; //for checks when a bullet/tank collides
	[[nodiscard]] inline Circle* getRightPoint() const;

public:
	//virtual std::vector<std::string> getHazardTypes() const override; //matched with RectangularLightningHazard
	virtual std::unordered_map<std::string, float> getWeights() const override;

protected:
	virtual void specialEffectCircleCollision(const Circle*) override;

public:
	virtual bool validLocation() const override;
	virtual bool reasonableLocation() const override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "horizontal_lightning"; }

protected:
	virtual inline void drawBackground(bool pose, float alpha = 1.0f) const;
	virtual inline void drawBackgroundOutline(float alpha) const; //called by drawBackground()
	virtual inline void drawBolts_Pose(float alpha = 1.0f) const;

public:
	HorizontalLightningHazard(double xpos, double ypos, double width, double height);
	~HorizontalLightningHazard();
	static RectHazard* factory(const GenericFactoryConstructionData&);
	static RectHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
	virtual int getFactoryArgumentCount() const override { return 4; }
	virtual RectHazardConstructionTypes getConstructionType() const override { return RectHazardConstructionTypes::standardConstruction; }
	virtual RectFactoryInformation getFactoryInformation() const override { return { true, true, false, false, true }; }
};
