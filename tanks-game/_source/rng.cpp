#include "rng.h"

#include <chrono>

std::default_random_engine RNG::generator;
std::uniform_real_distribution<double> RNG::distribution(0, 1);

void RNG::Initialize(long long seed) {
	generator = std::default_random_engine(seed);
}

void RNG::Initialize() {
	RNG::Initialize(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

double RNG::randFunc() {
	return distribution(generator);
}

int RNG::randIntInRange(int min, int max) {
	return static_cast<int>(RNG::randFunc() * (max - min)) + min;
}

double RNG::randNumInRange(double min, double max) {
	return RNG::randFunc() * (max - min) + min;
}
float RNG::randFloatInRange(float min, float max) {
	return static_cast<float>(RNG::randFunc()) * (max - min) + min;
}
