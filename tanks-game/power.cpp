#include "power.h"

/*
std::vector<std::string> Power::getPowerTypes() const {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla", "supermix-vanilla", "supermix", "random-vanilla", "random" };
	return types;
}

std::unordered_map<std::string, float> Power::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "supermix-vanilla", 1.0f });
	weights.insert({ "supermix", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}
*/

std::vector<std::string> Power::getPowerAttributes() const {
	std::vector<std::string> attributes = std::vector<std::string>{ "stack", "mix" };
	return attributes;
}

//power notes:
/*
list of (eventual) powers:
|speed
|wallhack
|multishot
|bounce
|triple shot
|homing
|invincibility (tier 1)
|big (tier 2)
|megadeath (variable big)
the super (tier 3) (name WIP) (come on, of course the name isn't going to be temporary!)
|grenade
|fire
|banana
|godmode
|barrier
|shield (super barrier) (barrier health dependent on time left on power)


powers that are rare because the level wants them, not because they're good:
|mines
|blast
|shotgun
|tracking


possible uses:
|inversion (inverts turning of tank)
|annoying (is annoying)


other ideas, some of which are jokes, some of which are lesser jokes:
nukes (???) (the only thing I remember about its conception is that it would act like Snake's Final Smash in Brawl)
ghost (color would be partially transparent)
the ID changer
poison? (like, another bullet/hazard is hit and it stops) (tank hits poisoned tank and dies)
boomerang (non-refillable? except when the shot is destroyed)
shurikan (shouldn't be too hard, especially if blast gets in)
teleport? (will have a teleport hazard, but how would a powerup look like? the longer you hold down shoot the further you'll teleport?)
trickster (moves in circles around its path; imagine MewTwo's shadow ball)


other:
swarm (bullets circle around tank at a set distance) (modifiedMovementCanOnlyWorkIndividually = true?)
sword (melee power) (heck yeah, but would have to let Tank hold an object; shouldn't be too hard?)
lightsaber (melee power) (sword but can reflect bullets)
stationary turret superguard? (like multishot but turrets spaced around tank)
mine layer (shoot a regular bullet, every certain distance a mine is dropped)
|dev: ultrabounce (bounce but it pushes the walls back)
dev: spread (triple but shoots at the same point and spreads out)
dev: pusher (pushes walls and hazards (tank only))
dev: ring-shooter (shoots a ring of bullets; just looks amusing) (multishot but all the angles are the same)


invincibility series notes:
nothing (default): tank/hazard: tier 0 defense (importance -1), tier 0 offense (importance -1); bullet: tier 0 offense against tank/hazard (importance -1), tier 0 offense against bullet (importance -1), tier 0 defense (importance -1)
if bullet/tank/hazard defense == bullet offense, then both should die (should is keyword)
invincibility: tank/hazard: tier 2.5 defense (importance 0), tier 0 offense (importance 0); bullet: tier 1 offense against tank/hazard (importance 0), tier 1 offense against bullet (importance 0), tier 1 defense (importance 0)
big: tank/hazard: tier 0 defense (importance 0), tier 0 offense (importance 0); bullet: tier 2 offense against tank/hazard (importance 0), tier 2 offense against bullet (importance 0), tier 2 defense (importance 0)
super: tank/hazard: tier 3 defense (importance 0), tier 3 offense (importance 0); bullet: tier 3 offense against tank/hazard (importance 0), tier 3 offense against bullet (importance 0), tier 4 defense (importance 0)

nothing turret: tier 1.5 defense (importance 0), tier 0 offense (importance -1); bullet: tier 0 offense against tank/hazard (importance -1), tier 0 offense against bullet (importance -1), tier 0 defense (importance -1) //big shot can keep destroying more, super can't
joke (ID changer): tank/hazard: tier 0 defense (importance 0), tier 0 offense (importance 0); bullet: tier -1 offense against tank/hazard (importance 1), tier -1 offense against bullet (importance 1), tier -1 defense (importance 0) //cannot kill regular tank or bullet, but will change its ID if it hits something (including hazard?!)
ghost: tank/hazard: tier 1 defense (importance 0), tier 0 offense (importance 0); bullet: tier 0 offense against tank/hazard (importance 0), tier -999 offense against bullet (importance 0?), tier 999 defense (importance 0) //will pass through other bullets, can kill regular tank, cannot kill regular bullet
poison: tank/hazard: tier 0 defense (importance 0), tier 1 offense (importance 0); bullet: tier 1 offense against tank/hazard (importance 0), tier -1 offense against bullet (importance 0), tier -1 defense (importance 0)
weak invincibility: tank/hazard: tier 1 defense (importance 0), tier 0 offense (importance 0); bullet: tier 0 offense against tank/hazard (importance 0), tier 0 offense against bullet (importance 0), tier 0 defense (importance 0)


things the powers need to access, override, and/or deal with (of bullets and tanks, probably hazards too):
color (needs a default in case it doesn't want to override the tank's color or mistakenly add to the color mix)
movement (wall collision n' stuff, ... and stuff)
shootcounts
shooting, obviously
state changes on a tank/hazard: ability to add random thing to tank/hazard (sword, for instance)
whether the power can be in a random level
whether the power can be in a random power or godmode
whether the power can be mixed?
if powers cannot be mixed on level, ability to persist over other powers (use VERY sparingly) (probably just have for the constructor, not for the entire power type (have as option though, just never use it))
ability to be permanently applied
ability to override duration
give tank a time limit before it can fire the new power, but must be overridable and dependent on default shooting speed
can leave out of bounds, both real and fake (fake in the case of a portal level situation: center can leave out of bounds, edges might, dividing walls can't)


JS power numbers (with names updated):
0 = speed, 1 = invincible, 2 = wallhack, 3 = trap, 4 = bounce, 5 = multishot, 6 = big, 7 = barrier, 8 = shotgun, 9 = mines, 10 = laser, 11 = fire, 12 = triple, 13 = blast, 14 = homing, 15 = tracking, 16 = grenade, 17 = banana, 18 = godmode, 19 = megadeath
trap obviously won't be a powerup this time and laser is really complex, so those won't get implemented
C++ order of creating the JS powers (probably):
speed, wallhack, multishot, bounce, homing, triple, invincible, big, megadeath, grenade, fire, blast, banana, godmode, mines, shotgun, tracking, barrier

*/
