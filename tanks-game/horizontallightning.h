#pragma once
#include "recthazard.h"
#include <vector>

struct LightningBolt {
	std::vector<float> positions; //positions is array of (x,y) points in range [0,1]
	int length; //positions.size()/2, unless it's uninitialized
	LightningBolt() { length = 0; } //don't use
	LightningBolt(int l) {
		length = l;
		positions.reserve(l*2);
	}
};

class HorizontalLightning : public RectHazard {
	//just called Lightning in JS Tanks
protected:
	double tickCount = 0;
	double tickCycle;
	bool currentlyActive;
	double* stateMultiplier; //length = 2 because bool bolt action
	//bool flexible; //worry about later

	const unsigned int maxBolts = 2; //this is maximum amount of normal bolts; the lightning can still make more bolts when it has to destroy a lot of bullets
	double lengthOfBolt = 4;
	std::vector<LightningBolt*> bolts; //is a vector of pointers instead of objects so resizes take less time
	double boltTick = 0;
	double boltCycle = 4; //how often bolts get refreshed
	virtual void refreshBolts(); //how often bolts get refreshed

private:
	static VertexArray* background_va;
	static VertexBuffer* background_vb;
	static IndexBuffer* background_ib;
	VertexArray* bolt_va;
	VertexBuffer* bolt_vb;
	//the bolt is just lines so only the length is needed when drawing (meaning no IndexBuffer needed)
	static bool initialized_GPU;
	virtual void streamBoltVertices(unsigned int boltNum); //(stream to bolt_vb)
public:
	static bool initializeGPU();
	void local_initializeGPU();
	static bool uninitializeGPU();
	void local_uninitializeGPU();

public:
	virtual ColorValueHolder getBackgroundColor();
	virtual ColorValueHolder getBoltColor();
	virtual std::string getName() { return getClassName(); };
	static std::string getClassName() { return "horizontal lightning"; };

	bool validLocation();

	void tick();
	void draw();
	void drawCPU();

	HorizontalLightning(double xpos, double ypos, double width, double height);
	//HorizontalLightning(double xpos, double ypos, double width, double height, bool flexible);
	~HorizontalLightning();
	static RectHazard* factory(int, std::string*);
};