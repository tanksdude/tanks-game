#pragma once
#include "leveleffect.h"
#include <vector>
#include <string>

class Level {
private:
	std::vector<LevelEffect*> effects;
public:
	virtual void initialize() = 0;
	virtual void tick() = 0;
	virtual void draw() = 0;
	static std::string powerAlternate(int position, int rand, std::string p1, std::string p2) {
		return ( (int)ceil(( (float)((position * 2) % 3)) / 3) + rand) % 2 == 0 ? p1 : p2;
		/*
		{0,   1,   2,  3} // start
		{0,   2,   4,  6} // *2
		{0,   2,   1,  0} // %3, then cast to float
		{0,  .6,  .3,  0} // /3, then add rand
		{0,  .6,  .3,  0} (rand=0) -> ceil -> {0, 1, 1, 0}
		{1, 1.6, 1.3,  1} (rand=1) -> ceil -> {1, 2, 2, 1}
		{0, 1, 1, 0} OR	{1, 0, 0, 1} // %2
		*/
	}
};

/*
TODO: separate levels from level effects?


list of (eventual) levels or level effects, I dunno:
random (just need to flesh it out once powers and hazards exist)
empty
invisibility? (no one appreciates it though)
wind
ice
mines




list of potential levels:
portal? (shouldn't be that hard if I do it correctly this time, right?)
maze


boring levels (it's a class (not literal C++ class... except maybe)/designation of levels):
hiding places (has mines?)
evened corridors
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





things the levels need to access, override, and/or deal with:




*/