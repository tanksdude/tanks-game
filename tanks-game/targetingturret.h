#pragma once
#include "stationaryturret.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class TargetingTurret : public StationaryTurret {
	//just called Turret in JS Tanks
protected:
	//double angle;
	//double tickCount = 0;
	//double tickCycle;
	//unsigned short currentState = 0;
	//unsigned short maxState;
	//double* stateMultiplier;
	//ColorValueHolder* stateColors;

	double turningIncrement = 128;
	bool targeting;
	double targetingX, targetingY; //not the x and y of the targeted tank; it's the x and y of the targeting reticule
	ColorValueHolder reticuleColors[2];
	double targetingCount = 0;
	long trackingID; //if ==this->getGameID(), then it's not tracking

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
	//virtual bool validLocation() override { return true; }
	virtual bool reasonableLocation() override;

	virtual bool canSeeTank(Tank*); //true if no walls obstruct any line of sight to tank
	virtual void turnTowardsTank(Tank*);
	virtual bool isPointedAt(Tank*);
	virtual ColorValueHolder getColor() const override;
	virtual ColorValueHolder getColor(short state) const override;
	virtual ColorValueHolder getReticuleColor() const;

	virtual std::string getName() const override { return getClassName(); }
	static std::string getClassName() { return "targeting_turret"; }

	virtual void tick();
	virtual void draw() const override;
	virtual void draw(double xpos, double ypos) const override;
	virtual void poseDraw() const override;
	virtual void drawCPU();

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
