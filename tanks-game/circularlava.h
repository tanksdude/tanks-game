#pragma once
#include "circlehazard.h"
#include "generalizedlava.h"

#include "vertexarray.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"

class CircularLava : public CircleHazard, public GeneralizedLava {
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
public:
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
	static std::string getClassName() { return "circular lava"; }

	virtual void tick();
	virtual void draw();
	virtual void drawCPU();

	CircularLava(double xpos, double ypos, double radius);
	~CircularLava();
	static CircleHazard* factory(int, std::string*);
};
