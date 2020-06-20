#pragma once
#include "recthazard.h"
#include "generalizedlava.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class RectangularLava : public RectHazard, public GeneralizedLava {
protected:
	virtual void pushNewBubble(double radius);

private:
	static VertexArray* background_va;
	static VertexBuffer* background_vb;
	static IndexBuffer* background_ib;
	static VertexArray* bubble_va;
	static VertexBuffer* bubble_vb;
	static IndexBuffer* bubble_ib;
	static bool initialized_GPU;
	
	static bool initializeGPU();
	static bool uninitializeGPU();

public:
	virtual double getDefaultOffense() { return .5; }
	virtual double getDefaultDefense() { return 999; }

	//virtual bool actuallyCollided(Tank*) { return true; }
	//bool modifiesTankCollision = true;
	virtual void modifiedTankCollision(Tank*) { return; }
	
	virtual bool actuallyCollided(Bullet* b) { return (b->velocity == 0); }
	//bool modifiesBulletCollision = true;
	virtual void modifiedBulletCollision(Bullet*) { return; }

	//virtual bool validLocation() { return true; }
	virtual bool reasonableLocation();

	virtual std::string getName() { return getClassName(); }
	static std::string getClassName() { return "rectangular lava"; }

	virtual void tick();
	virtual void draw();
	virtual void drawCPU();

	RectangularLava(double xpos, double ypos, double width, double height);
	~RectangularLava();
	static RectHazard* factory(int, std::string*);
	static int getFactoryArgumentCount() { return 4; }
	//static RectHazardConstructionTypes getConstructionType() { return RectHazardConstructionTypes::standardConstruction; }
};
