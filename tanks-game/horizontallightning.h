#pragma once
#include "recthazard.h"
#include <vector>
#include "lightningcommon.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class HorizontalLightning : public RectHazard {
	//just called Lightning in JS Tanks
protected:
	double tickCount = 0;
	double tickCycle;
	bool currentlyActive;
	double stateMultiplier[2]; //length = 2 because bool bolt action
	//bool flexible; //worry about later

	Circle* getLeftPoint(); //for checks when a bullet/tank collides
	Circle* getRightPoint();

	const unsigned int maxBolts = 2; //this is maximum amount of normal bolts; the lightning can make any number of bolts when it has to destroy a bullet or tank
	double lengthOfBolt;
	std::vector<LightningBolt*> bolts; //is a vector of pointers instead of objects so resizes take less time
	virtual void clearBolts(); //the vector holds pointers, so memory has to be freed
	double boltTick = 0;
	double boltCycle = 4; //how often bolts get refreshed
	bool boltsNeeded = false; //if the lightning hits something, this is changed, and no random bolts will be made; reset every boltCycle ticks
	virtual void refreshBolts(); //redraw the bolts
	virtual void refreshBolt(int num); //redraw a bolt
	virtual void simpleRefreshBolt(int num); //fast path for refreshing a bolt that goes from beginning to end
	virtual int getDefaultNumBoltPoints(double horzDist); //number of points that make up a bolt
	virtual void pushBolt(LightningBolt*, bool simpleRefresh);
	std::vector<long> targetedObjects;

private:
	static VertexArray* background_va;
	static VertexBuffer* background_vb;
	static IndexBuffer* background_ib;
	VertexArray* bolt_va;
	VertexBuffer* bolt_vb;
	//the bolt is just lines so only the length is needed when drawing (meaning no IndexBuffer needed)
	int bolt_vb_length;
	void local_reinitializeGPU(int length);
	static bool initialized_GPU;
	virtual void streamBoltVertices(unsigned int boltNum); //(stream to bolt_vb)
public:
	static bool initializeGPU();
	void local_initializeGPU();
	static bool uninitializeGPU();
	void local_uninitializeGPU();

public:
	virtual double getDefaultOffense() { return .5; } //1.5?
	virtual double getDefaultDefense() { return 999; }

	virtual bool actuallyCollided(Tank*) { return currentlyActive; }
	//bool modifiesTankCollision = true;
	virtual void modifiedTankCollision(Tank*) { return; }
	//bool hasSpecialEffectTankCollision = true;
	virtual void specialEffectTankCollision(Tank*);

	virtual bool actuallyCollided(Bullet*) { return currentlyActive; }
	//bool modifiesBulletCollision = true;
	virtual void modifiedBulletCollision(Bullet*) { return; }
	//bool hasSpecialEffectBulletCollision = true;
	virtual void specialEffectBulletCollision(Bullet*);
protected:
	virtual void specialEffectCircleCollision(Circle*); //tanks and bullets are both circles, so calculating the bolt positions would be the same

public:
	virtual ColorValueHolder getBackgroundColor();
	virtual ColorValueHolder getBoltColor();
	virtual std::string getName() { return getClassName(); }
	static std::string getClassName() { return "horizontal lightning"; }

	virtual bool validLocation();
	virtual bool reasonableLocation();

	virtual void tick();
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
