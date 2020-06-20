#pragma once
class Power;

#include <string>
#include <vector>
#include <unordered_map>
#include "colorvalueholder.h"
#include "tankpower.h"
#include "bulletpower.h"

class Power {
public:
	virtual std::vector<std::string> getPowerTypes(); //defined in source file in case I change some values
	virtual std::unordered_map<std::string, float> getWeights(); //intended range: (0,1]
	virtual std::vector<std::string> getPowerAttributes(); //stuff like "can stack", "can mix" ("can X" is only a suggestion)

	virtual std::string getName() = 0;
	//static std::string getClassName(); //tankpowers and bulletpowers use this (so they can have the same color), so make sure to define it!
	virtual ColorValueHolder getColor() = 0;
	//static ColorValueHolder getClassColor();
	virtual double getColorImportance() { return 0; }
	//static double getClassColorImportance();

	virtual TankPower* makeTankPower() = 0;
	virtual BulletPower* makeBulletPower() = 0;
	//virtual HazardPower* makeHazardPower() = 0;

	virtual ~Power() { return; }
	static Power* factory();
};

/*
list of (eventual) powers:
|speed
|wallhack
|multishot
|bounce
|triple shot (any want for shotgun?)
|homing
|invincibility (tier 1)
|big (tier 2)
|mega-death (variable big)
the super (tier 3) (name WIP) (come on, of course the name isn't going to be temporary!) (save for later because there's little point in including it as of now)
|grenade?
|fire?
|banana
|godmode
barrier (if hit, provides one extra hitpoint (basically life, but might add some differences))
super life/shield/barrier (barrier health dependent on time left on power)
swarm (check notes if you forgot, me) (modifiedMovementCanOnlyWorkIndividually = true?)


powers that are rare because the level wants them, not because they're good:
mines
|blast
tracking?


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


other ideas, some of which are jokes, some of which are lesser jokes:
nukes (???) (the only thing I remember about its conception is that it would act like Snake's Final Smash in Brawl)
sword (melee power) (heck yeah, but would have to let Tank hold an object; shouldn't be too hard?)
ghost (color would be partially transparent)
the ID changer
poison? (like, another bullet/hazard is hit and it stops) (tank hits poisoned tank and dies)
boomerang (non-refillable? except when the shot is destroyed)
shurikan (shouldn't be too hard, especially if blast gets in)
teleport? (will have a teleport hazard, but how would a powerup look like? the longer you hold down shoot the further you'll teleport?)
ring-shooter? (shoots a ring of bullets; just looks amusing) (multishot but all the angles are the same)
trickster (moves in circles around its path; imagine MewTwo's shadow ball)


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
can leave out-of-bounds, both real and fake (fake in the case of a portal level situation: center can leave out-of-bounds, edges might, dividing walls can't)


if you don't know where I got these names: that's okay, I do (more or less)
but they're from my JS game "Tanks: Battle of the Circles" (hosted on uncreativeusername.neocities.org), and this is a C++ port/remake/remaster/sequel (closest to sequel, closer to remake, barely a remaster, hardly a port)
*/
