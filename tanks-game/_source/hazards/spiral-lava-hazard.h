#pragma once
#include "../rect-hazard.h"

#include "../constants.h"

class SpiralLavaHazard : public RectHazard {
	//the base part is just a wall
protected:
	ColorValueHolder color;

	int maxLavaBlobs; //don't know what else to call them...
	std::vector<Game_ID> lavaBlobIDs;

	double tickCount;
	double tickCycle;
	bool currentlyActive;
	bool moveClockwise;
	double maxLavaDist;
	double lavaAngleRotate; //default 180deg

protected:
	virtual CircleHazard* makeLavaBlob(int blobNum) const;
	void pushLavaBlob(int blobNum); //shouldn't need to be virtual

	virtual inline double getLavaBlobAngle(int blobNum, double tickValue) const;
	virtual inline double getLavaBlobDist(double tickValue) const;
	virtual inline double getLavaBlobRadius() const;

	virtual bool canReachTanks() const; //used during randomization factory (in reasonableLocation())

public:
	virtual std::vector<std::string> getHazardTypes() const override {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
		return types;
	}
	virtual std::unordered_map<std::string, float> getWeights() const override;

	virtual RectHazardCollisionType getCollisionType() const override { return RectHazardCollisionType::solid; }

protected:
	virtual float getDefaultOffense() const override { return 0; }
	virtual float getDefaultDefense() const override { return DESTRUCTION_TIER; }

public:
	//virtual bool validLocation() const override { return true; }
	virtual bool reasonableLocation() const override;
	virtual void initialize() override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "spiral_lava"; }

	virtual void tick() override;
	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;

public:
	SpiralLavaHazard(double xpos, double ypos, double width);
	SpiralLavaHazard(double xpos, double ypos, double width, int lavaCount);
	SpiralLavaHazard(double xpos, double ypos, double width, int lavaCount, double lavaDist, double lavaRotateAngle);
	~SpiralLavaHazard();
	static RectHazard* factory(const GenericFactoryConstructionData&);
	static RectHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, const GenericFactoryConstructionData&);
	virtual int getFactoryArgumentCount() const override { return 3; }
	virtual RectHazardConstructionTypes getConstructionType() const override { return RectHazardConstructionTypes::standardSquareConstruction; }
	virtual RectFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
