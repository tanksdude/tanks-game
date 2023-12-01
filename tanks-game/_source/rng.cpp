#include "rng.h"

#include <chrono>

std::default_random_engine GameRNG::generator;
std::uniform_real_distribution<double> GameRNG::distribution(0, 1);

std::default_random_engine VisualRNG::generator;
std::uniform_real_distribution<double> VisualRNG::distribution(0, 1);

std::default_random_engine LevelRNG::generator;
std::uniform_real_distribution<double> LevelRNG::distribution(0, 1);

void GameRNG::Initialize(long long seed) {
	generator = std::default_random_engine(seed);
}
void VisualRNG::Initialize(long long seed) {
	generator = std::default_random_engine(seed);
}
void LevelRNG::Initialize(long long seed) {
	generator = std::default_random_engine(seed);
}

void GameRNG::Initialize() {
	Initialize(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}
void VisualRNG::Initialize() {
	Initialize(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}
void LevelRNG::Initialize() {
	Initialize(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

double GameRNG::randFunc() {
	return distribution(generator);
}
double VisualRNG::randFunc() {
	return distribution(generator);
}
double LevelRNG::randFunc() {
	return distribution(generator);
}

int GameRNG::randIntInRange(int min, int max) {
	return static_cast<int>(randFunc() * (max - min)) + min;
}
int VisualRNG::randIntInRange(int min, int max) {
	return static_cast<int>(randFunc() * (max - min)) + min;
}
int LevelRNG::randIntInRange(int min, int max) {
	return static_cast<int>(randFunc() * (max - min)) + min;
}

double GameRNG::randNumInRange(double min, double max) {
	return randFunc() * (max - min) + min;
}
double VisualRNG::randNumInRange(double min, double max) {
	return randFunc() * (max - min) + min;
}
double LevelRNG::randNumInRange(double min, double max) {
	return randFunc() * (max - min) + min;
}
float GameRNG::randFloatInRange(float min, float max) {
	return static_cast<float>(randFunc()) * (max - min) + min;
}
float VisualRNG::randFloatInRange(float min, float max) {
	return static_cast<float>(randFunc()) * (max - min) + min;
}
float LevelRNG::randFloatInRange(float min, float max) {
	return static_cast<float>(randFunc()) * (max - min) + min;
}
