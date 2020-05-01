#pragma once
#include "recthazard.h"
#include "lavabubble.h"
#include <vector>

//TODO: decide if circular lava will exist (it would be really simple, so may as well?)
class Lava : public RectHazard {
protected:
	double tickCount = 0;
	double tickCycle; //this is used to make the brightness of the lava vary sinusoidally (yes, that's a word)
	
	const unsigned int maxBubbles = 8; //there wasn't a limit in JS Tanks because the bubbles were rare (1/400 per tick, 100Hz), but there should be, whether it's met or not
	std::vector<LavaBubble*> bubbles;
	double bubbleChance = 1.0/400; //chance to spawn bubble this tick
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
	//void local_initializeGPU();
	static bool uninitializeGPU();
	//void local_uninitializeGPU();

public:
	virtual ColorValueHolder getBackgroundColor();
	virtual ColorValueHolder getBubbleColor(LavaBubble* bubble);
	virtual std::string getName() { return getClassName(); };
	static std::string getClassName() { return "lava"; };

	void tick();
	void draw();
	void drawCPU();

	Lava(double xpos, double ypos, double width, double height);
	~Lava();
	static RectHazard* factory(int, std::string*);
};