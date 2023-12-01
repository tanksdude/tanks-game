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
	//TODO: powers that can't "stack" should be enforced; for example, two fires should only produce 4 bullets, meaning the second one is just ignored
	//that would require a very large rewrite, so save it for a sequel
	//also there should be some kind of "mix but not well" attribute for something like swarm + blast (mainly to stop deceleration making things boring)
}

//power notes:
/*
power ideas:
the ID changer
boomerang (non-refillable? except when the shot is destroyed)
sword (melee power) (heck yeah, but would have to let Tank hold an object; shouldn't be too hard?)
lightsaber (melee power) (sword but can reflect bullets)


JS power numbers (with names updated):
0 = speed, 1 = invincible, 2 = wallhack, 3 = trap (not a real power), 4 = bounce, 5 = multishot, 6 = big, 7 = barrier, 8 = shotgun, 9 = mines, 10 = laser (unimplemented), 11 = fire, 12 = triple, 13 = blast, 14 = homing, 15 = tracking, 16 = grenade, 17 = banana, 18 = godmode, 19 = megadeath
trap obviously won't be a powerup this time and laser is really complex, so those won't get implemented
C++ order of creating the JS powers (probably):
speed, wallhack, multishot, bounce, homing, triple, invincible, big, megadeath, grenade, fire, blast, banana, godmode, mines, shotgun, tracking, barrier


random fun facts:
can you tell how the earlier the JS power, the more unique it was? I generally thought "that'd be cool to implement" and did it; since there was nothing else at the time, they were very different from each other
when first making multishot (in JS), it started out as adding a single cannon facing behind, but that had very limited use; then it was four cannons, but it wasn't quite useful enough; then it was 8 cannons, and everything was good... though performance would suffer sometimes
although traps officially existed in JS, a few subtle "traps" were added: for example, concealed powerups level didn't have powerup mixing, so if you wanted the good stuff at the center, you needed to be careful; almost no one did, learned about user interaction that day
in JS, I added homing, tracking, grenade, and banana all in one day; that might be the single largest creativity burst ever for this whole project
in JS, I don't think big bullets passing through each other was intentional, it was just how it was hacked into the invincible system, but it stayed that way because I liked it; not changed back as an option because it's complicated
powerups resetting tank shooting cooldown doesn't happen in this version because of a few reasons: mainly I forgot, secondarily it doesn't fit in cleanly (though I could probably add a system to do so), tertiary reason is it's more fun without it
in JS, powers didn't have color for a *long* time (until fire, I think); after so long, I was running out of shades of gray to represent powers, and I was the only one who could tell them apart (and I think I struggled with big and something else), so colors were finally added... and now I'm starting to run out of colors in this version
original color choosing: speed is blue because Sonic, invincible is white because I think that's a "generic" invincible color, and wallhack is yellow because Super Saiyan and Super Sonic; everything else did have some thought put into it but nothing as concrete (others with some thought: big is red because red is a "strong" color in games, fire is orange because... fire, and banana is gold because banana is awesome and deserves gold; continued: mines is black to make turning difficult (because I thought it would be a funny balancing mechanism) and godmode is rainbow because why shouldn't it be)
swarm was one of the first new powerups I thought up for the C++ version, and it ended up being one of the last powerups implemented

*/
