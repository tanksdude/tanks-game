#pragma once

class GameManager {
	friend class DeveloperManager;
	friend class ResetThings;
private:
	static long nextID;
	static double tickCount; //double in case physics happens at a variable rate
	static void Reset();
public:
	static void Tick();
	static double getTickCount();
	static long getNextID();
	static void Initialize();

private:
	GameManager() {}
	GameManager(const GameManager&) {}
};
