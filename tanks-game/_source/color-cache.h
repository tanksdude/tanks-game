#pragma once
#include <string>
#include <unordered_map>
#include "color-value-holder.h"

//currently, color cache identifiers are separated by "|", so don't put that in your power's name
//TODO: when powers get added, check through every power's identifier to make sure it doesn't have a "|", exit program if it does (make sure that's only for debug builds)

//TODO: string hashing is still slow, is there a way around that?
//probable solution: at startup, give every power a unique int ID, hash based on the int
//problem: how to mix? maybe every color could instead be a set bit, which would need a BigInt library for >64 colors, however this wouldn't account for multiple of the same power
//continued: so is the only solution then to set a hard limit on the number of powers? if there's a cap of like 8 for a specific power, then bit/int alignment would be pretty easy... yeah, that's probably how it would be done in real optimized software
//continued continued: if the cap is 2^8 per power, each bullet would need to store <num of powers> bytes just for color... at that point, hashing a string would probably be faster

class ColorCacheBullet {
protected:
	static std::unordered_map<std::string, ColorValueHolder> cachedColors;

public:
	static void invalidateColors(); //called at the start of a new frame

	static bool colorExists(const std::string& identifier);
	static const ColorValueHolder& getColor(const std::string& identifier);
	[[nodiscard]] static const ColorValueHolder& insertColor(const std::string& identifier, const ColorValueHolder* colors, int num);

private:
	ColorCacheBullet() = delete;
	ColorCacheBullet(const ColorCacheBullet&) = delete;
};
