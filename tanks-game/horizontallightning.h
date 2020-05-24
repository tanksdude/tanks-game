#pragma once
#include "recthazard.h"
#include <vector>

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

struct LightningBolt {
	std::vector<float> positions; //positions is array of (x,y) points (for easy vertex streaming)
	int length; //positions.size()/2, unless it's uninitialized
	LightningBolt() { length = 0; } //don't use
	LightningBolt(int l) { //try not to use
		length = l;
		positions.reserve(l*2);
	}
	LightningBolt(float startX, float startY, float endX, float endY, int l) {
		length = l;
		positions.reserve(l*2);
		positions.push_back(startX); positions.push_back(startY);
		for (int i = 1; i < l-1; i++) {
			positions.push_back(startX + (endX-startX) * float(i)/(l-1));
			positions.push_back(startY + (endY-startY) * float(i)/(l-1));
		}
		positions.push_back(endX); positions.push_back(endY);
	}
};

class HorizontalLightning : public RectHazard {
	//just called Lightning in JS Tanks
protected:
	double tickCount = 0;
	double tickCycle;
	bool currentlyActive;
	double stateMultiplier[2]; //length = 2 because bool bolt action
	//bool flexible; //worry about later

	Circle* leftSide; //for checks when a bullet/tank collides
	Circle* rightSide;

	const unsigned int maxBolts = 2; //this is maximum amount of normal bolts; the lightning can make any number of bolts when it has to destroy a bullet or tank
	double lengthOfBolt;
	std::vector<LightningBolt*> bolts; //is a vector of pointers instead of objects so resizes take less time
	double boltTick = 0;
	double boltCycle = 4; //how often bolts get refreshed
	bool boltsNeeded = false; //if the lightning hits something, this is changed, and no random bolts will be made; reset every boltCycle ticks
	virtual void refreshBolts(); //redraw the bolts
	virtual void refreshBolt(int num); //redraw a bolt
	virtual void simpleRefreshBolt(int num); //fast path for refreshing a bolt that goes from beginning to end
	virtual int getDefaultNumBoltPoints(double horzDist); //number of points that make up a bolt
	virtual void pushBolt(LightningBolt*, bool simpleRefresh);
	virtual void clearBolts(); //the vector holds pointers, so memory has to be freed
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
	virtual RectFactoryInformation getFactoryInformation() {
		return { true, true, false, false, true };
	}
};
