#include "generalizedlightning.h"
#include "backgroundrect.h"
#include "colormixer.h"
#include "mylib.h"
#include "constants.h"
#include <math.h>
//#include <stdexcept>
#include <algorithm> //std::copy
#include <iostream>

int GeneralizedLightning::getDefaultNumBoltPoints(double horzDist) const {
	int boltPoints = ceil(horzDist / lengthOfBolt); //not floor because the last point is the edge of the lightning area
	return (boltPoints < 2 ? 2 : boltPoints);
}

GeneralizedLightning::~GeneralizedLightning() {
	clearBolts();
}

void GeneralizedLightning::tick() {
	if (!validLocation()) {
		tickCount = 0;
		currentlyActive = false;
		targetedObjects.clear();
		clearBolts();
		return;
	}

	tickCount++;
	if (tickCount >= tickCycle * stateMultiplier[currentlyActive]) {
		if (tickCycle * stateMultiplier[currentlyActive] <= 0) {
			tickCount = 0;
			currentlyActive = true;
		} else {
			tickCount -= tickCycle * stateMultiplier[currentlyActive];
			currentlyActive = !currentlyActive;
		}
	}
	
	if (currentlyActive) {
		if (boltTick >= boltCycle) {
			//hazard tick comes before collision, therefore there will be more bolt refreshes after this if a bullet/tank collides
			targetedObjects.clear();
			boltsNeeded = false;
			clearBolts();
			pushDefaultBolt(maxBolts, true);
			boltTick = 0;
		}
		boltTick++;
	} else {
		boltTick = boltCycle; //start at boltCycle to force a refresh as soon as possible
	}
}

void GeneralizedLightning::clearBolts() {
	for (int i = 0; i < bolts.size(); i++) {
		delete bolts[i];
	}
	bolts.clear();
}

ColorValueHolder GeneralizedLightning::getBackgroundColor() const {
	if (currentlyActive) {
		return ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(.75f, .75f, .75f), .25);
	}
	return ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(.75f, .75f, .75f), .25*constrain<double>(tickCount/(tickCycle*stateMultiplier[currentlyActive]), 0, 1));
}

ColorValueHolder GeneralizedLightning::getBoltColor() const {
	return ColorValueHolder(0xBB/255.0, 1.0f, 1.0f);
}
