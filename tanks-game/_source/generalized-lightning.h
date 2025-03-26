#pragma once
#include <vector>

#include "constants.h"

#include "color-value-holder.h"

class GeneralizedLightning {
protected:
	struct LightningBolt {
		std::vector<float> positions; //positions is array of (x,y) points
		int length; //positions.size()/2, unless it's uninitialized
		LightningBolt(int l) { //try not to use
			length = l;
			positions.reserve(l*2);
		}
		LightningBolt(float startX, float startY, float endX, float endY, int l) {
			length = l;
			positions.reserve(l*2);
			positions.push_back(startX); positions.push_back(startY);
			for (int i = 1; i < l-1; i++) {
				positions.push_back(startX + (endX-startX) * float(i)/(l-1));
				positions.push_back(startY + (endY-startY) * float(i)/(l-1));
			}
			positions.push_back(endX); positions.push_back(endY);
		}
		//LightningBolt(const LightningBolt&); //no need
	private:
		LightningBolt() = delete;
	};

protected:
	double tickCount;
	double tickCycle;
	bool currentlyActive;
	double stateMultiplier[2]; //length = 2 because bool bolt action

	unsigned int maxBolts; //this is maximum amount of normal bolts; the lightning can make any number of bolts when it has to destroy a bullet or tank
	float lengthOfBolt;
	std::vector<LightningBolt*> bolts;
	virtual void clearBolts();
	double boltTick;
	double boltCycle; //= 4; //how often bolts get refreshed
	bool boltsNeeded; //= false; //if the lightning hits something, this is changed, and no random bolts will be made; resets every boltCycle ticks

	virtual void refreshBolt(LightningBolt*) const = 0; //"redraw" a bolt //this is the hardest thing to generalize, so... copy and paste
	virtual int getDefaultNumBoltPoints(float horzDist) const; //number of points that make up a bolt
	virtual void pushBolt(LightningBolt*) = 0;
	virtual void pushDefaultBolt(int num, bool randomize) = 0; //randomize should be true all of the time
	std::vector<Game_ID> targetedObjects;

public:
	virtual ColorValueHolder getBackgroundColor() const;
	virtual ColorValueHolder getBackgroundColor_Pose() const;
	virtual ColorValueHolder getBoltColor() const;

	virtual bool validLocation() const = 0;
	virtual void tick();

	virtual ~GeneralizedLightning();
};
