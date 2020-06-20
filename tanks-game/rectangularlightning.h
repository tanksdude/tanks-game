#pragma once
#include "recthazard.h"
#include "generalizedlightning.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class RectangularLightning : public RectHazard, public GeneralizedLightning {
	//called LightningZone in JS Tanks
protected:
	Circle* getCenterPoint(); //for checks when a bullet/tank collides (needs to be a function in case the lightning changes size or position)

	//unsigned int maxBolts; // = 1;
	virtual void refreshBolt(int num);
	virtual void refreshBolt(int num, double smaller, double larger);
	//virtual int getDefaultNumBoltPoints(double horzDist);
	virtual void pushBolt(LightningBolt*);
	virtual void pushDefaultBolt(int num, bool randomize); //randomize should be true all of the time

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
	void streamBoltVertices(unsigned int boltNum); //(stream to bolt_vb)

	static bool initializeGPU();
	void local_initializeGPU();
	static bool uninitializeGPU();
	void local_uninitializeGPU();

public:
	virtual double getDefaultOffense() override { return .5; } //1.5?
	virtual double getDefaultDefense() override { return 999; }

	virtual bool actuallyCollided(Tank*) override { return currentlyActive; }
	//bool modifiesTankCollision = true;
	virtual void modifiedTankCollision(Tank*) override { return; }
	//bool hasSpecialEffectTankCollision = true;
	virtual void specialEffectTankCollision(Tank*) override;

	virtual bool actuallyCollided(Bullet*) override { return currentlyActive; }
	//bool modifiesBulletCollision = true;
	virtual void modifiedBulletCollision(Bullet*) override { return; }
	//bool hasSpecialEffectBulletCollision = true;
	virtual void specialEffectBulletCollision(Bullet*) override;
protected:
	virtual void specialEffectCircleCollision(Circle*); //tanks and bullets are both circles, so calculating the bolt positions would be the same

public:
	virtual std::string getName() { return getClassName(); }
	static std::string getClassName() { return "rectangular lightning"; }

	virtual bool validLocation() override;
	virtual bool reasonableLocation() override;

	virtual void tick() { GeneralizedLightning::tick(); }
	virtual void draw();
	virtual void drawCPU();

protected:
	RectangularLightning(double xpos, double ypos, double width, double height, bool noGPU); //doesn't initialize GPU
public:
	RectangularLightning(double xpos, double ypos, double width, double height);
	~RectangularLightning();
	static RectHazard* factory(int, std::string*);
	static int getFactoryArgumentCount() { return 4; }
	//static RectHazardConstructionTypes getConstructionType() { return RectHazardConstructionTypes::standardConstruction; }
};
