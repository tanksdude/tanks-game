#pragma once
#include "rectangularlightning.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class HorizontalLightning : public RectangularLightning {
	//just called Lightning in JS Tanks
protected:
	//double tickCount = 0;
	//double tickCycle;
	//bool currentlyActive;
	//double stateMultiplier[2]; //length = 2 because bool bolt action
	//bool flexible; //worry about later

	Circle* getLeftPoint(); //for checks when a bullet/tank collides
	Circle* getRightPoint();

	//unsigned int maxBolts; // = 2;
	//double lengthOfBolt;
	//std::vector<LightningBolt*> bolts;
	//virtual void clearBolts();
	//double boltTick = 0;
	//double boltCycle = 4; //how often bolts get refreshed
	//bool boltsNeeded = false; //if the lightning hits something, this is changed, and no random bolts will be made; reset every boltCycle ticks
	virtual void refreshBolts(); //redraw the bolts
	virtual void refreshBolt(int num); //redraw a bolt
	virtual void simpleRefreshBolt(int num); //fast path for refreshing a bolt that goes from beginning to end
	virtual int getDefaultNumBoltPoints(double horzDist); //number of points that make up a bolt
	virtual void pushBolt(LightningBolt*, bool simpleRefresh);
	virtual void pushDefaultBolt(int num, bool randomize);
	//std::vector<long> targetedObjects;

private:
	static VertexArray* background_va;
	static VertexBuffer* background_vb;
	static IndexBuffer* background_ib;
	VertexArray* bolt_va;
	VertexBuffer* bolt_vb;
	//the bolt is just lines so only the length is needed when drawing (meaning no IndexBuffer needed)
	int bolt_vb_length;
	static bool initialized_GPU;
	void local_reinitializeGPU(int length);
	void streamBoltVertices(unsigned int boltNum);

	static bool initializeGPU();
	void local_initializeGPU();
	static bool uninitializeGPU();
	void local_uninitializeGPU();

public:
	//virtual double getDefaultOffense() { return .5; } //1.5?
	//virtual double getDefaultDefense() { return 999; }

	//virtual void specialEffectTankCollision(Tank*);
	//virtual void specialEffectBulletCollision(Bullet*);
protected:
	virtual void specialEffectCircleCollision(Circle*);

public:
	//virtual ColorValueHolder getBackgroundColor();
	//virtual ColorValueHolder getBoltColor();
	virtual std::string getName() { return getClassName(); }
	static std::string getClassName() { return "horizontal lightning"; }

	virtual bool validLocation();
	virtual bool reasonableLocation();

	//virtual void tick(); //it's the same as RectangularLightning's tick()
	virtual void draw();
	virtual void drawCPU();

	HorizontalLightning(double xpos, double ypos, double width, double height);
	//HorizontalLightning(double xpos, double ypos, double width, double height, bool flexible);
	~HorizontalLightning();
	static RectHazard* factory(int, std::string*);
	static int getFactoryArgumentCount() { return 4; }
	//static RectHazardConstructionTypes getConstructionType() { return RectHazardConstructionTypes::standardConstruction; }
	virtual RectFactoryInformation getFactoryInformation() {
		return { true, true, false, false, true };
	}
};
