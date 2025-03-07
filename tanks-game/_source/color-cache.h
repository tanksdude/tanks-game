#pragma once
#include <string>
#include <unordered_map>
#include "color-value-holder.h"

//currently, color cache identifiers are separated by "|", so don't put that in your power's name

//TODO: string hashing is awfully slow, to the point where the performance gained from this is practically zero
//probable solution: at startup, give every power a unique int ID, hash based on the int
//problem: just that approach limits powers to a single global color... but that's an acceptable trade-off; I think properly doing a color changing color requires generating a texture and basically clipping out just where the bullets are
//another problem: it works well for just one power, but how to mix? does it hash to another hashmap? ID=0 for "no further mixing", but it's not like a hashmap can store both other hashmaps and a color
//continued: basically a color mixture needs to generate a unique int ID, then hash based on that... so maybe use the string to hash to a unique int, which then hashes to a color? no, because the point is to not hash strings

/*
class ColorCachePowerSquare {
protected:
	static std::unordered_map<std::string, ColorValueHolder> cachedColors;

public:
	static bool colorExists(const std::string& identifier);
	static const ColorValueHolder& getColor(const std::string& identifier);
	[[nodiscard]] static const ColorValueHolder& insertColor(const std::string& identifier, const ColorValueHolder* colors, int num); //returns the inserted color

private:
	ColorCachePowerSquare() = delete;
	ColorCachePowerSquare(const ColorCachePowerSquare&) = delete;
};



class ColorCacheTank {
protected:
	static std::unordered_map<std::string, ColorValueHolder> cachedColors;

public:
	static bool colorExists(const std::string& identifier);
	static const ColorValueHolder& getColor(const std::string& identifier);
	[[nodiscard]] static const ColorValueHolder& insertColor(const std::string& identifier, const ColorValueHolder* colors, int num);

private:
	ColorCacheTank() = delete;
	ColorCacheTank(const ColorCacheTank&) = delete;
};
*/



class ColorCacheBullet {
protected:
	static std::unordered_map<std::string, ColorValueHolder> cachedColors;

public:
	static bool colorExists(const std::string& identifier);
	static const ColorValueHolder& getColor(const std::string& identifier);
	[[nodiscard]] static const ColorValueHolder& insertColor(const std::string& identifier, const ColorValueHolder* colors, int num);

private:
	ColorCacheBullet() = delete;
	ColorCacheBullet(const ColorCacheBullet&) = delete;
};
