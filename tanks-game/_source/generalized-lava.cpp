#include "generalized-lava.h"

#include "constants.h"
#include <cmath>
#include <algorithm> //std::copy
#include <limits> //std::numeric_limits<float>::infinity();
#include "rng.h"

#include "color-mixer.h"

//yes, the syntax is a little weird
GeneralizedLava::LavaBubble::LavaBubble(float radius, float x0, float y0, float x1, float y1, float tickStart) {
	r = radius;
	xStart = x0;
	xEnd = x1;
	yStart = y0;
	yEnd = y1;

	tickCount = 0;
	tickMax = tickStart;
	state = 0;

	float temp[] = { 1, 1, 1, std::numeric_limits<float>::infinity() }; //maybe use std::numeric_limits<float>::quiet_NaN()
	std::copy(temp, temp+4, stateMultiplier); //screw you, C++
}

GeneralizedLava::LavaBubble::LavaBubble(float radius, float x0, float y0, float x1, float y1, float tickStart, const float* tickMultiplier)
: LavaBubble(radius, x0, y0, x1, y1, tickStart) {
	std::copy(tickMultiplier, tickMultiplier + 3, stateMultiplier); //the last one isn't supposed to be modified (or used)
}

GeneralizedLava::LavaBubble::~LavaBubble() {
	//nothing
}

void GeneralizedLava::LavaBubble::tick() {
	tickCount++;
	while (tickCount >= tickMax * stateMultiplier[state] && state < 3) {
		tickCount -= tickMax * stateMultiplier[state];
		state++;
	}
}

bool GeneralizedLava::LavaBubble::isDead() const {
	return (state >= 3);
}

float GeneralizedLava::LavaBubble::getAlpha() const {
	switch (state) {
		case 0: return tickCount / (tickMax * stateMultiplier[0]) * .5;
		case 1: return .5;
		case 2: return (1 - tickCount / (tickMax * stateMultiplier[2])) * .5;
		default: return 0;
	}
}

float GeneralizedLava::LavaBubble::getR() const {
	return r;
}

float GeneralizedLava::LavaBubble::getX() const {
	switch (state) {
		case 0: return xStart;
		case 1: return xStart + (xEnd - xStart) * tickCount/(tickMax * stateMultiplier[1]);
		case 2: return xEnd;
		default: return xEnd;
	}
}

float GeneralizedLava::LavaBubble::getY() const {
	switch (state) {
		case 0: return yStart;
		case 1: return yStart + (yEnd - yStart) * tickCount/(tickMax * stateMultiplier[1]);
		case 2: return yEnd;
		default: return yEnd;
	}
}



ColorValueHolder GeneralizedLava::getBackgroundColor() const {
	//colors: red (#FF0000) and orange-red (#FFAA00) mixed
	return ColorMixer::mix(ColorValueHolder(1.0f, 0.0f, 0.0f), ColorValueHolder(1.0f, 0.875f, 0.0f),
	                       .625 + sin((2*PI) * (tickCount/tickCycle))/8 + cos((2*PI) * (tickCount/tickCycle) * 8)/8);
}

ColorValueHolder GeneralizedLava::getBackgroundColor_Pose() const {
	//color: orange-red-ish (#FF4400)
	return ColorValueHolder(1.0f, 0.25f, 0.0f);
}

ColorValueHolder GeneralizedLava::getBubbleColor(LavaBubble* bubble) const {
	//a bubble's natural color is white, but with an alpha of .5, but blending is expensive so it's just mixed with the lava background
	return ColorMixer::mix(ColorValueHolder(1.0f, 1.0f, 1.0f), getBackgroundColor(), 1.0f - bubble->getAlpha());
}

ColorValueHolder GeneralizedLava::getBubbleColor_Pose(LavaBubble* bubble) const {
	return ColorMixer::mix(ColorValueHolder(1.0f, 1.0f, 1.0f), getBackgroundColor(), .5); //TODO: allow getting the base alpha from a bubble
}

void GeneralizedLava::tick() {
	if (++tickCount >= tickCycle) [[unlikely]] {
		tickCount = 0;
	}

	if ((bubbles.size() < maxBubbles) && (RNG::randFunc() < bubbleChance)) {
		pushNewBubble(4); //possible radius: sqrt(w * h * 2)/50 or sqrt(r * r * 2)/50
	}

	for (int i = bubbles.size()-1; i >= 0; i--) {
		bubbles[i]->tick();
		if (bubbles[i]->isDead()) {
			delete bubbles[i];
			bubbles.erase(bubbles.begin() + i);
		}
	}
}

GeneralizedLava::~GeneralizedLava() {
	for (int i = 0; i < bubbles.size(); i++) {
		delete bubbles[i];
	}
	//bubbles.clear(); //not necessary
}
