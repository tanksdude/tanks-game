#include "level-effect.h"

/*
std::vector<std::string> LevelEffect::getLevelEffectTypes() const {
	std::vector<std::string> types = std::vector<std::string>{ "vanilla", "random-vanilla", "random" };
	return types;
}

std::unordered_map<std::string, float> LevelEffect::getWeights() const {
	std::unordered_map<std::string, float> weights;
	weights.insert({ "vanilla", 1.0f });
	weights.insert({ "random-vanilla", 1.0f });
	weights.insert({ "random", 1.0f });
	return weights;
}
*/

//level effect notes:
/*
list of (eventual) level effects:
|wall invisibility
|wind
|ice (no one really likes it, as expected)
|minefield
|respawning powerups
gravity/magnet (which is it?)


other:
respawning walls?
moving walls... of some sort
ultrafast shooting?
darkness?

I need more ideas for level effects...


JS level effect numbers (with names updated):
0 = wind, 1 = ice, 2 = minefield
invisible walls and portal were not real level effects
C++ order of creating the JS level effects (probably):
wind, minefield, ice, invisible walls


random fun facts:
like hazards and most powers, level effects were added because I had some sudden inspiration
some people when first exposed to the ice level effect (in JS) assumed it was lag
when I first started planning for levels and level effects (in C++), I figured portal would be difficult but doable; now I'm sure it's doable with time, but it'd be *a lot* of effort
I thought a bunch new level effects would get added this version, but that never happened... only thing new is respawning powerups; was hoping other levels would include a level effect like wind, but that also never happened...

*/
