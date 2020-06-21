#pragma once
#include "recthazard.h"
#include "generalizednobulletzone.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class RectangularNoBulletZone : public RectHazard, public GeneralizedNoBulletZone {
	//called NoShotZone in JS Tanks (because bullets were called "shots")
private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
	static bool initialized_GPU;
	
	static bool initializeGPU();
	static bool uninitializeGPU();

public:
	virtual double getDefaultOffense() override { return 999; }
	virtual double getDefaultDefense() override { return 999; }

	virtual bool actuallyCollided(Tank*) override { return false; }
	//bool modifiesTankCollision = true;
	virtual void modifiedTankCollision(Tank*) override { return; }
	
	virtual bool actuallyCollided(Bullet*) override { return true; }
	//bool modifiesBulletCollision = true;
	virtual void modifiedBulletCollision(Bullet*) override { return; } //TODO: should the bullet be destroyed here?

	//virtual bool validLocation() override { return true; }
	virtual bool reasonableLocation() override;

	//ColorValueHolder getColor(); //in GeneralizedNoBulletZone

	virtual std::string getName() { return getClassName(); }
	static std::string getClassName() { return "no bullet zone"; }

	virtual void tick();
	virtual void draw();
	virtual void drawCPU();

	RectangularNoBulletZone(double xpos, double ypos, double width, double height);
	~RectangularNoBulletZone();
	static RectHazard* factory(int, std::string*);
	static RectHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv);
	virtual int getFactoryArgumentCount() override { return 4; }
	virtual RectHazardConstructionTypes getConstructionType() override { return RectHazardConstructionTypes::standardConstruction; }
	virtual RectFactoryInformation getFactoryInformation() override { return { false, false, false, false, false }; }
};
