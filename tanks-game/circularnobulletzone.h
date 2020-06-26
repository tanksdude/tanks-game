#pragma once
#include "circlehazard.h"
#include "generalizednobulletzone.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class CircularNoBulletZone : public CircleHazard, public GeneralizedNoBulletZone {
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

	CircularNoBulletZone(double xpos, double ypos, double radius);
	~CircularNoBulletZone();
	static CircleHazard* factory(int, std::string*);
	static CircleHazard* randomizingFactory(double x_start, double y_start, double area_width, double area_height, int argc, std::string* argv);
	virtual int getFactoryArgumentCount() override { return 3; }
	virtual CircleHazardConstructionTypes getConstructionType() override { return CircleHazardConstructionTypes::radiusRequired; }
	virtual CircleFactoryInformation getFactoryInformation() override { return { false, false, false, false, false }; }
};
