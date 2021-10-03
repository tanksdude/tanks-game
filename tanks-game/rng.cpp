#include "rng.h"
#include <limits>
#include <cstdlib> //pretty sure this is auto-included in C++

const int RNG::MAX_RAND_NUM = std::numeric_limits<int>::max();
std::mt19937 RNG::generator;
std::uniform_int_distribution<int> RNG::distribution;

//apparently, I don't know how to initialize a mt19937 based on the current time
//do you need very large time differences to see seed differences? I really don't know
//switching back to the C way just to have different seeds

void RNG::Initialize(long long seed) {
	//std::mt19937 generator(seed); //mersenne_twister_engine
	//std::uniform_int_distribution<int> distribution(0, RNG::MAX_RAND_NUM);
	srand(seed);
}

double RNG::randFunc() {
	//return double(distribution(generator)) / (double(RNG::MAX_RAND_NUM) + 1);
	return double(rand()) / (double(RAND_MAX) + 1);
}

double RNG::randFunc2() {
	//return double(distribution(generator)) / double(RNG::MAX_RAND_NUM);
	return double(rand()) / double(RAND_MAX);
}
