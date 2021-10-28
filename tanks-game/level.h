#pragma once
class Level;

#include "leveleffect.h"
#include "colorvalueholder.h"
#include <vector>
#include <string>
#include <unordered_map>

class Level {
	friend class LevelManager; //actually needs this unlike the other managers because the manager has more control

protected:
	std::vector<LevelEffect*> effects; //cleared in Level's destructor

public:
	LevelEffect* getLevelEffect(int i) const { return effects[i]; }
	int getNumEffects() const { return effects.size(); }

	virtual std::string getName() const = 0;
	virtual ColorValueHolder getDefaultColor() const = 0;
	virtual std::vector<std::string> getLevelTypes() const;
	virtual std::unordered_map<std::string, float> getWeights() const; //intended range: (0,1]

	virtual void initialize() = 0;
	virtual void tick() { return; }
	void tickLevelEffects();
	void doLevelEffects();
	virtual void draw() const { return; }
	void drawLevelEffects() const;

	virtual ~Level();
	static Level* factory(int, std::string*);
};
