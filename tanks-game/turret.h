#pragma once
#include "stationaryturret.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class Turret : public StationaryTurret {
	//should it be called "regular turret" or something? "targeting turret"?
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
	//TODO: rest of stuff

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
	virtual ColorValueHolder getColor() override;
	virtual ColorValueHolder getColor(short state) override;
	virtual ColorValueHolder getReticuleColor();

	virtual std::string getName() { return getClassName(); }
	static std::string getClassName() { return "turret"; }

	virtual void tick();
	virtual void draw();
	virtual void drawCPU();

protected:
	Turret(double xpos, double ypos, double angle, bool noGPU);
public:
	Turret(double xpos, double ypos, double angle);
	Turret(double xpos, double ypos, double angle, double radius);
	virtual ~Turret();
	static CircleHazard* factory(int, std::string*);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv);
	//virtual int getFactoryArgumentCount() override { return 3; }
	//virtual CircleHazardConstructionTypes getConstructionType() override { return CircleHazardConstructionTypes::angleRequired; }
	//virtual CircleFactoryInformation getFactoryInformation() override { return { false, false, false, false, false }; }
};
