#pragma once
#include <random>

class RNG {
private:
	static std::default_random_engine generator;
	static std::uniform_real_distribution<double> distribution;

public:
	static void Initialize(long long seed);
	static void Initialize();

	[[nodiscard]] static double randFunc(); //[0,1)
	[[nodiscard]] static int randIntInRange(int min, int max); //[min, max)
	[[nodiscard]] static double randNumInRange(double min, double max); //[min, max)
	[[nodiscard]] static float randFloatInRange(float min, float max); //[min, max)

private:
	RNG() = delete;
	RNG(const RNG&) = delete;
};
