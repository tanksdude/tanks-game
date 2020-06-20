#pragma once
#include "recthazard.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class NoBulletZone : public RectHazard {
	//called NoShotZone in JS Tanks (because bullets were called "shots")
private:
	static VertexArray* va;
	static VertexBuffer* vb;
	static IndexBuffer* ib;
	static bool initialized_GPU;
	
	static bool initializeGPU();
	static bool uninitializeGPU();

public:
	virtual double getDefaultOffense() { return 999; }
	virtual double getDefaultDefense() { return 999; }

	virtual bool actuallyCollided(Tank*) { return false; }
	//bool modifiesTankCollision = true;
	virtual void modifiedTankCollision(Tank*) { return; }
	
	virtual bool actuallyCollided(Bullet*) { return true; }
	//bool modifiesBulletCollision = true;
	virtual void modifiedBulletCollision(Bullet*) { return; } //TODO: should the bullet be destroyed here?

	//virtual bool validLocation() { return true; }
	virtual bool reasonableLocation();

	ColorValueHolder getColor();

	virtual std::string getName() { return getClassName(); }
	static std::string getClassName() { return "no bullet zone"; }

	virtual void tick();
	virtual void draw();
	virtual void drawCPU();

	NoBulletZone(double xpos, double ypos, double width, double height);
	~NoBulletZone();
	static RectHazard* factory(int, std::string*);
	static int getFactoryArgumentCount() { return 4; }
	//static RectHazardConstructionTypes getConstructionType() { return RectHazardConstructionTypes::standardConstruction; }
};
