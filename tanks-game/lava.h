#pragma once
#include "recthazard.h"
#include "generalizedlava.h"
#include <vector>

class Lava : public RectHazard, public GeneralizedLava {
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

	//bool modifiesTankCollision = true;
	virtual bool actuallyCollided(Tank*) { return true; }
	virtual void modifiedTankCollision(Tank*) { return; }
	
	//bool modifiesBulletCollision = true;
	virtual bool actuallyCollided(Bullet* b) { return (b->velocity == 0); }
	virtual void modifiedBulletCollision(Bullet*) { return; }

	virtual std::string getName() { return getClassName(); }
	static std::string getClassName() { return "rectangular lava"; }

	virtual void tick();
	virtual void draw();
	virtual void drawCPU();

	Lava(double xpos, double ypos, double width, double height);
	~Lava();
	static RectHazard* factory(int, std::string*);
};
