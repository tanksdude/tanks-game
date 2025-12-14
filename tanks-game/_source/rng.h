#pragma once
#include <random>

//game logic
class GameRNG {
private:
	static std::default_random_engine generator;
	static std::uniform_real_distribution<double> distribution;

public:
	static void Initialize(long long seed);
	static void Initialize();

	[[nodiscard]] static double randFunc(); //[0,1)
	[[nodiscard]] static inline float randFuncf() { return static_cast<float>(randFunc()); }
	[[nodiscard]] static int randIntInRange(int min, int max); //[min, max)
	[[nodiscard]] static double randNumInRange(double min, double max); //[min, max)
	[[nodiscard]] static float randFloatInRange(float min, float max); //[min, max)

private:
	GameRNG() = delete;
	GameRNG(const GameRNG&) = delete;
};

//visual stuff like lightning bolts
class VisualRNG {
private:
	static std::default_random_engine generator;
	static std::uniform_real_distribution<double> distribution;

public:
	static void Initialize(long long seed);
	static void Initialize();

	[[nodiscard]] static double randFunc(); //[0,1)
	[[nodiscard]] static inline float randFuncf() { return static_cast<float>(randFunc()); }
	[[nodiscard]] static int randIntInRange(int min, int max); //[min, max)
	[[nodiscard]] static double randNumInRange(double min, double max); //[min, max)
	[[nodiscard]] static float randFloatInRange(float min, float max); //[min, max)

private:
	VisualRNG() = delete;
	VisualRNG(const VisualRNG&) = delete;
};

//choosing levels and hazard randomization
class LevelRNG {
private:
	static std::default_random_engine generator;
	static std::uniform_real_distribution<double> distribution;

public:
	static void Initialize(long long seed);
	static void Initialize();

	[[nodiscard]] static double randFunc(); //[0,1)
	[[nodiscard]] static inline float randFuncf() { return static_cast<float>(randFunc()); }
	[[nodiscard]] static int randIntInRange(int min, int max); //[min, max)
	[[nodiscard]] static double randNumInRange(double min, double max); //[min, max)
	[[nodiscard]] static float randFloatInRange(float min, float max); //[min, max)

private:
	LevelRNG() = delete;
	LevelRNG(const LevelRNG&) = delete;
};
