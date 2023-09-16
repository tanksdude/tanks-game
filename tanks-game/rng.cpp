#include "rng.h"
#include <chrono>

std::default_random_engine RNG::generator;
std::uniform_real_distribution<double> RNG::distribution(0, 1);

void RNG::Initialize(long long seed) {
	generator = std::default_random_engine(seed);
	//std::uniform_real_distribution<double> distribution(0, 1);
}

void RNG::Initialize() {
	RNG::Initialize(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

double RNG::randFunc() {
	return distribution(generator);
}

int RNG::randNumInRange(int min, int max) {
	return int(RNG::randFunc() * (max - min)) + min;
}
