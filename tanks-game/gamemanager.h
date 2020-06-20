#pragma once

class GameManager {
	friend class DeveloperManager;
	friend class ResetThings;
private:
	static long nextID;
	static double tickCount; //double in case physics happens at a variable rate
	static void reset();
public:
	static void tick();
	static double getTickCount();
	static long getNextID();
	static void initialize();
};
