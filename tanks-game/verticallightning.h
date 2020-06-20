#pragma once
#include "rectangularlightning.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class VerticalLightning : public RectangularLightning {
	//called VLightning in JS Tanks
protected:
	//bool flexible; //worry about later

	Circle* getTopPoint(); //for checks when a bullet/tank collides
	Circle* getBottomPoint();

	//unsigned int maxBolts; // = 2;
	virtual void refreshBolt(int num); //uses RectangularLightning::refreshBolt
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
	static std::string getClassName() { return "vertical lightning"; }

	virtual bool validLocation() override;
	virtual bool reasonableLocation() override;

	//virtual void tick();
	virtual void draw();
	virtual void drawCPU();

	VerticalLightning(double xpos, double ypos, double width, double height);
	//VerticalLightning(double xpos, double ypos, double width, double height, bool flexible);
	~VerticalLightning();
	static RectHazard* factory(int, std::string*);
	static int getFactoryArgumentCount() { return 4; }
	//static RectHazardConstructionTypes getConstructionType() { return RectHazardConstructionTypes::standardConstruction; }
	virtual RectFactoryInformation getFactoryInformation() {
		return { false, false, true, true, true };
	}
};
