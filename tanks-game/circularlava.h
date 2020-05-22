#pragma once
#include "circlehazard.h"
#include "lavabubble.h"
#include <vector>

class CircularLava : public CircleHazard {
	//see RectangularLava for some explanations
protected:
	double tickCount = 0;
	double tickCycle;
	
	const unsigned int maxBubbles = 8;
	std::vector<LavaBubble*> bubbles;
	double bubbleChance = 1.0/400;
	void pushNewBubble(double radius);

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

protected:
	virtual ColorValueHolder getBubbleColor(LavaBubble* bubble);
public:
	virtual double getDefaultOffense() { return .5; }
	virtual double getDefaultDefense() { return 999; }

	//bool modifiesTankCollision = true;
	virtual bool actuallyCollided(Tank*) { return true; }
	virtual void modifiedTankCollision(Tank*) { return; }
	
	//bool modifiesBulletCollision = true;
	virtual bool actuallyCollided(Bullet* b) { return (b->velocity == 0); }
	virtual void modifiedBulletCollision(Bullet*) { return; }

	virtual ColorValueHolder getBackgroundColor();
	virtual std::string getName() { return getClassName(); };
	static std::string getClassName() { return "circular lava"; };

	void tick();
	void draw();
	void drawCPU();

	CircularLava(double xpos, double ypos, double radius);
	~CircularLava();
	static CircleHazard* factory(int, std::string*);
};
