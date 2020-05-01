#pragma once

struct LavaBubble {
	//idea: extends Circle so some collision can be done with it, though I have no idea why it would be needed
	friend class Lava;
private:
	//floats, not doubles, since it's not like you can see the difference anyway (also less implicit casting)
	float r;
	float xStart; //[r,w-r]
	float yStart; //[r,h-r]
	float xEnd; //[r,w-r]
	float yEnd; //[r,h-r]

	int tickCount; //int instead of a double because it makes no sense to have a double when tick++ is all that happens
	double tickMax; //double just to avoid an explicit cast for finding the alpha when bubble is rising or falling

	double stateMultiplier[4];
	unsigned int state;
	// 0 = rising
	// 1 = moving
	// 2 = "popping" (falling? fading?)
	// 3 = dead (don't use)

	void tick();
	bool isDead();
	double getAlpha();
	float getX();
	float getY();

	LavaBubble(float radius, float x0, float y0, float x1, float y1, double tickStart);
	LavaBubble(float radius, float x0, float y0, float x1, float y1, double tickStart, double* tickMultiplier); //tickMultiplier length = 4
};