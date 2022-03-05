#pragma once
#include <random>

class RNG {
private:
	//static const int MAX_RAND_NUM;
	static std::default_random_engine generator;
	static std::uniform_real_distribution<double> distribution;
	//static std::uniform_int_distribution<int> distribution; //for [a,b] instead of [a,b), but it just doesn't matter

public:
	static void Initialize(long long seed);
	static void Initialize();

	static double randFunc(); //[0,1)
	//static double randFunc2(); //[0,1]
	static int randNumInRange(int min, int max); //[min, max)
};
