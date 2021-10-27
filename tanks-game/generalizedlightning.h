#pragma once
#include <vector>
#include "colorvalueholder.h"
#include "constants.h"

class GeneralizedLightning {
protected:
	//see notes in GeneralizedLava about nested classes
	struct LightningBolt {
		std::vector<float> positions; //positions is array of (x,y) points (for easy vertex streaming)
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
		LightningBolt() {}
	};

protected:
	double tickCount = 0;
	double tickCycle;
	bool currentlyActive;
	double stateMultiplier[2]; //length = 2 because bool bolt action

	unsigned int maxBolts; //this is maximum amount of normal bolts; the lightning can make any number of bolts when it has to destroy a bullet or tank
	double lengthOfBolt;
	std::vector<LightningBolt*> bolts; //is a vector of pointers instead of objects so resizes take less time
	virtual void clearBolts(); //the vector holds pointers, so memory has to be freed
	double boltTick = 0;
	double boltCycle = 4; //how often bolts get refreshed
	bool boltsNeeded = false; //if the lightning hits something, this is changed, and no random bolts will be made; resets every boltCycle ticks
	virtual void refreshBolt(int num) = 0; //"redraw" a bolt //this is the hardest thing to generalize, so... copy and paste
	virtual int getDefaultNumBoltPoints(double horzDist) const; //number of points that make up a bolt
	virtual void pushBolt(LightningBolt*) = 0;
	virtual void pushDefaultBolt(int num, bool randomize) = 0; //randomize should be true all of the time
	std::vector<Game_ID> targetedObjects;

	//the lightning is supposed to override this stuff:
	/*
public:
	virtual double getDefaultOffense() { return .5; } //1.5?
	virtual double getDefaultDefense() { return 999; }

	virtual bool actuallyCollided(Tank*) { return currentlyActive; }
	//bool modifiesTankCollision = true;
	virtual void modifiedTankCollision(Tank*) { return; }
	//bool hasSpecialEffectTankCollision = true;
	virtual void specialEffectTankCollision(Tank*);

	virtual bool actuallyCollided(Bullet*) { return currentlyActive; }
	//bool modifiesBulletCollision = true;
	virtual void modifiedBulletCollision(Bullet*) { return; }
	//bool hasSpecialEffectBulletCollision = true;
	virtual void specialEffectBulletCollision(Bullet*);
protected:
	virtual void specialEffectCircleCollision(Circle*); //tanks and bullets are both circles, so calculating the bolt positions would be the same
	*/

public:
	virtual ColorValueHolder getBackgroundColor() const;
	virtual ColorValueHolder getBoltColor() const;

	virtual bool validLocation() = 0;

	virtual void tick();

public:
	virtual ~GeneralizedLightning();
};
