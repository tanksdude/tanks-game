#include "hazard.h"

//hazard notes:
/*
some hazards aren't bad

list of (eventual) hazards:
|stationary turret
|targeting turret
patrolling turret
|rectangular/circular lightning
|horizontal lightning
|vertical lightning
|lava
|no bullet zone
teleport zone
pipes
trap?


other:
cloud (what would this do? spit out some powerups every now and then?) (it's *not* another lightning, that's for sure)
	(maybe it eats a bullet and spits out a powerup)
flag for capture the flag (base where the tank spawns; bringing it back gives godmode or something)
wall hazards (it's a wall, except it's a hazard) (dev purposes only)
	maybe it can be pushed or something
magnetic hazard of some kind (when bullet or something is near it, shoots out some electricity (not lightning; there's a difference) and pulls it in)
tank swapper (swaps tanks every so often) (could also teleport them to some random place)


list of possible hazards, joke or not:
the reflecktor (bounces/reflects tanks when they hit it)


JS hazard numbers (with names updated):
0 = targeting turret, 1 = horizontal lightning, 2 = lava (rectangular), 3 = stationary turret, 4 = vertical lightning, 5 = rectangular lightning, 6 = no bullet zone, 7 = patrolling turret (unimplemented)
quite the lack of circular hazards
C++ order of creating the JS hazards (probably):
stationary turret, horizontal lightning, lava, rectangular lightning, vertical lightning, no bullet zone, targeting turret

*/
