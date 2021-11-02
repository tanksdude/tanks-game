#pragma once
#include "stationaryturret.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class TargetingTurret : public StationaryTurret {
	//just called Turret in JS Tanks
protected:
	//double angle;
	//double tickCount;
	//double tickCycle;
	//unsigned int currentState;
	//unsigned int maxState;
	//double* stateMultiplier;
	//ColorValueHolder* stateColors;

	double turningIncrement = 128;
	bool targeting;
	double targetingX, targetingY; //not the x and y of the targeted tank; it's the x and y of the targeting reticule
	ColorValueHolder reticuleColors[2];
	double targetingCount;
	Game_ID trackingID; //if ==this->getGameID(), then it's not tracking
	virtual inline void updateTrackingPos(const Tank*, bool pointedAt);

private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
	static VertexArray* cannon_va;
	static VertexBuffer* cannon_vb;
	static VertexArray* reticule_va;
	static VertexBuffer* reticule_vb;
	static bool initialized_GPU;

	static bool initializeGPU();
	static bool uninitializeGPU();

public:
	virtual void turnTowardsTank(const Tank*);
	virtual bool canSeeTank(const Tank*) const override; //true if no walls obstruct any line of sight to tank
	virtual bool isPointedAt(const Tank*) const;
	virtual ColorValueHolder getColor() const override;
	virtual ColorValueHolder getColor(int state) const override;
	virtual ColorValueHolder getReticuleColor() const;

	//virtual bool validLocation() const override { return true; }
	virtual bool reasonableLocation() const override;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "targeting_turret"; }

	virtual void tick() override;
	virtual void draw() const override;
	virtual void draw(DrawingLayers) const override;
	virtual void poseDraw() const override;
	virtual void poseDraw(DrawingLayers) const override;
	virtual void ghostDraw(float alpha) const override;
	virtual void ghostDraw(DrawingLayers, float alpha) const override;
	//virtual void drawCPU() const override;

private:
	inline void drawBody(float alpha = 1.0f) const;
	inline void drawOutline(float alpha = 1.0f) const;
	inline void drawBarrel(float alpha = 1.0f) const;
	inline void drawReticule(float alpha = 1.0f) const;

protected:
	TargetingTurret(double xpos, double ypos, double angle, bool noGPU);
public:
	TargetingTurret(double xpos, double ypos, double angle);
	TargetingTurret(double xpos, double ypos, double angle, double radius);
	virtual ~TargetingTurret();
	static CircleHazard* factory(int, std::string*);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv);
	//virtual int getFactoryArgumentCount() const override { return 3; }
	//virtual CircleHazardConstructionTypes getConstructionType() const override { return CircleHazardConstructionTypes::angleRequired; }
	//virtual CircleFactoryInformation getFactoryInformation() const override { return { false, false, false, false, false }; }
};
