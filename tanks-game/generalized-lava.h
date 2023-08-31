#pragma once
#include "color-value-holder.h"
#include <vector>

class GeneralizedLava {
protected:
	//note to anyone deciding to use nested classes: simply "struct LavaBubble;" is enough here
	//the struct can then be given a header-like definition at the top of the source file, then actual code at the bottom, if that's what you want
	struct LavaBubble {
	protected:
		//floats, not doubles, since it's not like you can see the difference anyway (also less implicit casting)
		float r;
		float xStart; //[0,1] (rect) or [-1,-1] (circle)
		float yStart; //[0,1] (rect) or [-1,-1] (circle)
		float xEnd;   //[0,1] (rect) or [-1,-1] (circle)
		float yEnd;   //[0,1] (rect) or [-1,-1] (circle)
		//if circle, x and y are constrained by the circle's edges (sine and cosine are involved)

		int tickCount;
		float tickMax; //float just to avoid an explicit cast for finding the alpha when bubble is rising or falling

		float stateMultiplier[4];
		unsigned int state;
		// 0 = rising
		// 1 = moving
		// 2 = "popping" (falling? fading?)
		// 3 = dead (don't use)

	public:
		virtual void tick();
		bool isDead() const;
		virtual float getAlpha() const;
		virtual float getR() const;
		virtual float getX() const;
		virtual float getY() const;
		//I don't know what would extend these functions, but whatever, doesn't hurt to make them virtual

		LavaBubble(float radius, float x0, float y0, float x1, float y1, float tickStart);
		LavaBubble(float radius, float x0, float y0, float x1, float y1, float tickStart, float* tickMultiplier); //tickMultiplier length = 3
		//LavaBubble(const LavaBubble&); //no need
		virtual ~LavaBubble();

	private:
		LavaBubble() = delete;
	};

protected:
	double tickCount;
	double tickCycle; //this is used to make the brightness of the lava vary sinusoidally

	const unsigned int maxBubbles = 8; //there wasn't a limit in JS Tanks because the bubbles were rare (1/400 per tick, 100Hz), but there should be, whether it's met or not
	std::vector<LavaBubble*> bubbles;
	double bubbleChance; //= 1.0/400 (chance to spawn bubble this tick)
	virtual void pushNewBubble(double radius) = 0;

public:
	virtual ColorValueHolder getBackgroundColor() const;
	virtual ColorValueHolder getBackgroundColor_Pose() const;
	virtual ColorValueHolder getBubbleColor(LavaBubble* bubble) const;
	virtual ColorValueHolder getBubbleColor_Pose(LavaBubble* bubble) const;

	virtual void tick();

	virtual ~GeneralizedLava();
};
