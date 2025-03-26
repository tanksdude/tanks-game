#include "generalized-lightning.h"

#include "constants.h"
#include <cmath>
//#include <stdexcept>
#include <algorithm> //std::copy, std::clamp, std::max
#include <iostream>

#include "color-mixer.h"
#include "background-rect.h"

int GeneralizedLightning::getDefaultNumBoltPoints(float horzDist) const {
	int boltPoints = ceil(horzDist / lengthOfBolt); //not floor because the last point is the edge of the lightning area
	return std::max(2, boltPoints);
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
	if (tickCount >= tickCycle * stateMultiplier[currentlyActive]) [[unlikely]] {
		if (tickCycle * stateMultiplier[currentlyActive] <= 0) [[unlikely]] {
			tickCount = 0;
			currentlyActive = true;
		} else {
			tickCount -= tickCycle * stateMultiplier[currentlyActive];
			currentlyActive = !currentlyActive;
		}
	}

	if (currentlyActive) {
		if (++boltTick >= boltCycle) {
			//hazard tick comes before collision, therefore there will be more bolt refreshes after this if a bullet/tank collides
			targetedObjects.clear();
			boltsNeeded = false;
			clearBolts();
			pushDefaultBolt(maxBolts, true);
			boltTick = 0;
		}
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
		return ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(0.75f, 0.75f, 0.75f), .25f);
	}
	return ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(0.75f, 0.75f, 0.75f), .25f*std::clamp<float>(tickCount/(tickCycle*stateMultiplier[currentlyActive]), 0, 1));
}

ColorValueHolder GeneralizedLightning::getBackgroundColor_Pose() const {
	//getBackgroundColor when currentlyActive = true
	return ColorMixer::mix(BackgroundRect::getBackColor(), ColorValueHolder(0.75f, 0.75f, 0.75f), .25f);
}

ColorValueHolder GeneralizedLightning::getBoltColor() const {
	return ColorValueHolder(0xBB/255.0, 1.0f, 1.0f);
}
