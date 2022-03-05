#include "rng.h"
//#include <limits>
#include <cstdlib> //pretty sure this is auto-included in C++
#include <chrono>

//const int RNG::MAX_RAND_NUM = std::numeric_limits<int>::max();
std::default_random_engine RNG::generator;
std::uniform_real_distribution<double> RNG::distribution;
//std::uniform_int_distribution<int> RNG::distribution;

//protip: don't use a mt19937

void RNG::Initialize(long long seed) {
	//srand(seed);
	generator = std::default_random_engine(seed);
	std::uniform_int_distribution<int> distribution(0, 1);
}

void RNG::Initialize() {
	RNG::Initialize(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

double RNG::randFunc() {
	//return double(distribution(generator)) / (double(RNG::MAX_RAND_NUM) + 1);
	//return double(rand()) / (double(RAND_MAX) + 1);
	return distribution(generator);
}

/*
double RNG::randFunc2() {
	//return double(distribution(generator)) / double(RNG::MAX_RAND_NUM);
	//return double(rand()) / double(RAND_MAX);
	return distribution(generator); //not [0,1] but whatever, it's not that important
}
*/

int RNG::randNumInRange(int min, int max) {
	return int(RNG::randFunc() * (max - min)) + min;
}
