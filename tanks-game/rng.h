#pragma once
#include <random>

class RNG {
private:
	static const int MAX_RAND_NUM;
	static std::mt19937 generator;
	static std::uniform_int_distribution<int> distribution;
public:
	static void Initialize(long long seed);
	static double randFunc(); //[0,1)
	static double randFunc2(); //[0,1]
};
