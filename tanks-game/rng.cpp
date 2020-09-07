#include "rng.h"
const int RNG::MAX_RAND_NUM = 2147483647;
std::mt19937 RNG::generator;
std::uniform_int_distribution<int> RNG::distribution;

void RNG::Initialize(unsigned int seed) {
	std::mt19937 generator(seed); //mersenne_twister_engine
	std::uniform_int_distribution<int> distribution(0, RNG::MAX_RAND_NUM);
}

double RNG::randFunc() {
	return double(distribution(generator)) / (double(RNG::MAX_RAND_NUM) + 1);
}

double RNG::randFunc2() {
	return double(distribution(generator)) / double(RNG::MAX_RAND_NUM);
}