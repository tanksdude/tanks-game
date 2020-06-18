#pragma once
#include "rectangularlightning.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class HorizontalLightning : public RectangularLightning {
	//just called Lightning in JS Tanks
protected:
	//bool flexible; //worry about later

	Circle* getLeftPoint(); //for checks when a bullet/tank collides
	Circle* getRightPoint();

	//unsigned int maxBolts; // = 2;
	virtual void refreshBolt(int num);
	virtual void simpleRefreshBolt(int num); //fast path for refreshing a bolt that goes from beginning to end
	//virtual int getDefaultNumBoltPoints(double horzDist);
	virtual void pushBolt(LightningBolt* l) { pushBolt(l, false); }
	virtual void pushBolt(LightningBolt*, bool simpleRefresh);
	virtual void pushDefaultBolt(int num, bool randomize);

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

protected:
	virtual void specialEffectCircleCollision(Circle*);

public:
	virtual std::string getName() { return getClassName(); }
	static std::string getClassName() { return "horizontal lightning"; }

	virtual bool validLocation();
	virtual bool reasonableLocation();

	//virtual void tick();
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
