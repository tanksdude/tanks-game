#pragma once
#include "leveleffect.h"
#include <vector>
#include <string>

class Level {
	friend class LevelManager; //actually needs this unlike the other managers because the manager has more control
protected:
	std::vector<LevelEffect*> effects;
	int getNumEffects() { return effects.size(); } //kinda pointless but "clean code"
public:
	virtual std::string getName() = 0;
	virtual std::vector<std::string> getLevelTypes() {
		std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random" };
		return types;
	}
	virtual void initialize() = 0;
	virtual void tick() { return; }
	virtual void draw() { return; }

	static Level* factory();
	virtual ~Level() { return; }
};

/*
TODO: separate levels from level effects? (... is this not the case?)

list of (eventual) levels or level effects, I dunno:
|random (just need to flesh it out once powers and hazards exist)
|empty
invisibility? (no one appreciates it though)
wind
ice (no one really likes it, as expected)
mines


list of potential levels:
portal? (shouldn't be that hard if I do it correctly this time, right?)
maze


boring levels:
hiding places (has mines?)
|evened corridors
few obstacles (homing level)
turret level (kinda boring)
concealed powers (needs some spice to become good)
many hazards
lightning corners (needs spice, but is a kinda fun level)


test levels (purpose: to showcase some functionality):
one wall (corner collision, heck yeah!)
one room with each powerup, isolated, permanent
test bullet priorities? level controls tanks for ease of viewing
one room with each hazard (and necessary other materials, if needed)
one room with each level effect ('cuz why not at this point)
turret room: two turrets; one has distance-based targeting (just selection), one has angle-based targeting (just selection)


"other":
team mode! (for two tanks...)

*/
